#include "Model.h"
#include "VertexInformation.h"
#include "SOIL2.H"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h> // Post processing flags

GLuint CreateTexture(const char* path, std::string dir);

Model::Model(std::string path)
{
	this->LoadModel(path);
}

void Model::Draw(const CompiledShader& shader, const glm::vec3& position, const glm::vec3& xRot, const glm::vec3& yRot, const glm::vec3& zRot, const glm::vec3& scale)
{
	for (unsigned int i = 0; i < this->meshes.size(); i++)
	{
		this->meshes[i].Draw(shader, position, xRot, yRot, zRot, scale);
	}
}

bool Model::LoadModel(std::string path)
{
	Assimp::Importer importer;

	unsigned int flags = 0;
	flags |= aiProcess_Triangulate; // Triangulates the faces (AKA: if there are models with faces > 3, it will turn them into triangles for us)
	flags |= aiProcess_JoinIdenticalVertices; // Joins identical vertex data
	flags |= aiProcess_GenSmoothNormals; // Generates smooth normals for all vertices in the mesh

	const aiScene* scene = importer.ReadFile(path, flags);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Failed to load model!" << std::endl;
		return false;
	}

	this->directory = path.substr(0, path.find_last_of('\\')) + "\\";
	this->fileName = path.substr(path.find_last_of('\\') + 1, path.length());

	this->LoadAssimpNode(scene->mRootNode, scene);
	return true;
}


void Model::LoadAssimpNode(aiNode* node, const aiScene* scene)
{
	// Process assimp meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->LoadAssimpMesh(mesh, scene));
	}

	// Recursivley processes child nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		this->LoadAssimpNode(node->mChildren[i], scene);
	}
}

std::string Model::GetFullPath() const
{
	return this->directory + this->fileName;
}

Mesh Model::LoadAssimpMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<sVertex> vertices;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		sVertex vertex;
		vertex.x = mesh->mVertices[i].x;
		vertex.y = mesh->mVertices[i].y;
		vertex.z = mesh->mVertices[i].z;

		vertex.nx = mesh->mNormals[i].x;
		vertex.ny = mesh->mNormals[i].y;
		vertex.nz = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0])
		{
			vertex.u0 = mesh->mTextureCoords[0][i].x;
			vertex.v0 = mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
	}

	std::vector<sTriangle> faces;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		sTriangle face;
		aiFace assimpFace = mesh->mFaces[i];
		if (assimpFace.mNumIndices != 3)
		{
			std::cout << "Face is not a triangle!" << std::endl;
		}

		for (unsigned int j = 0; j < assimpFace.mNumIndices; j++)
		{
			face.vertIndex[j] = assimpFace.mIndices[j];
		}

		faces.push_back(face);
	}

	std::vector<Texture> textures;
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		// Sampler names should adhere to the following convention:
		// Diffuse: texure_diffuseN
		// Specular: texture_specularN
		// Normal: texture_normalN
		// Where N = texture numbers

		for (Texture texture : this->LoadAssimpMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse"))
		{
			this->loadedTextures.insert(std::make_pair(texture.path.C_Str(), texture));
			textures.push_back(texture);
		}

		for (Texture texture : this->LoadAssimpMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular"))
		{
			this->loadedTextures.insert(std::make_pair(texture.path.C_Str(), texture));
			textures.push_back(texture);
		}
	}

	return Mesh(vertices, faces, textures);
}

std::vector<Texture> Model::LoadAssimpMaterialTextures(aiMaterial* material, aiTextureType type, std::string name)
{
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString str;
		material->GetTexture(type, i, &str);

		bool isTextureLoaded = this->loadedTextures.find(str.C_Str()) != this->loadedTextures.end();
		if (!isTextureLoaded)
		{
			Texture texture;
			texture.id = CreateTexture(str.C_Str(), this->directory);
			texture.type = name;
			texture.path = str;

			textures.push_back(texture);
		}
	}

	return textures;
}

GLuint CreateTexture(const char* path, std::string dir)
{
	std::string fileName = std::string(path);
	fileName = dir + fileName;

	// Generate a texture ID for ourselves
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	uint8_t* data = SOIL_load_image(fileName.c_str(), &width, &height, 0, SOIL_LOAD_RGB); // Read image file

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); // Tell OpenGL about our image (passes in the width, height and image data (pixels))
	glGenerateMipmap(GL_TEXTURE_2D);

	// Texure parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	SOIL_free_image_data(data);

	return textureID;
}