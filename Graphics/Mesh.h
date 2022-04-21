#pragma once

#include "GLCommon.h"
#include "Camera.h"
#include "VertexInformation.h"
#include "CompiledShader.h"

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp> 
#include <glm/mat4x4.hpp> 
#include <glm/gtc/matrix_transform.hpp> 

// Represents a mesh that can be drawn to the screen
class Mesh 
{
public:
	std::vector<sVertex> vertices;
	std::vector<sTriangle> faces;
	std::vector<Texture> textures;

	Mesh(std::vector<sVertex> vertices, std::vector<sTriangle> faces, std::vector<Texture> textures);

	// Draws this mesh to the screen
	void Draw(const CompiledShader& shader, const glm::vec3& position, const glm::vec3& xRot, const glm::vec3& yRot, const glm::vec3& zRot, const glm::vec3& scale);

private:
	GLuint VAO, VBO, EBO;

	glm::vec3 offset;
	glm::vec3 orientation;
	float scale;

	void SetupMesh();
};