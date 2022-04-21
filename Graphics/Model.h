#pragma once

#include "CompiledShader.h"
#include "Mesh.h"

#include <assimp/scene.h> // Output data structure

#include <string>
#include <vector>
#include <map>

// Represents a model that is made up of multiple meshes
class Model
{
public:
	Model(std::string path);

	// Draw this model
	void Draw(const CompiledShader& shader, const glm::vec3& position, const glm::vec3& xRot, const glm::vec3& yRot, const glm::vec3& zRot, const glm::vec3& scale);

	// Loads the model from file
	bool LoadModel(std::string path);

	// Get the path of this model
	std::string GetFullPath() const;
private:
	// Loads an assimp node
	void LoadAssimpNode(aiNode* node, const aiScene* scene);

	// Loads a mesh from assimp
	Mesh LoadAssimpMesh(aiMesh* mesh, const aiScene* scene);

	// Loads assimp material textures int our texture struct
	std::vector<Texture> LoadAssimpMaterialTextures(aiMaterial* material, aiTextureType type, std::string name);

private:
	std::vector<Mesh> meshes; // Holds meshes that are part of this model
	std::map<std::string, Texture> loadedTextures; // Holds loaded textures so that we don't have to reload them
	std::string directory;
	std::string fileName;
};