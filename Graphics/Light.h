#pragma once

#include "GLCommon.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp> 
#include <glm/vec4.hpp> 

class Light
{

	enum LightType
	{
		POINT = 0,
		SPOT = 1,
		DIRECTIONAL = 2,
	};

public:

	Light(int index);
	~Light();

	// Modifies the position of this light
	void EditPosition(const GLuint& shaderID, float x, float y, float z, float w);

	// Modifies the diffuse of this light
	void EditDiffuse(const GLuint& shaderID, float x, float y, float z, float w);

	// Modifies the specular color and power of this light
	void EditSpecular(const GLuint& shaderID, float r, float g, float b, float power);

	// Modifies the attenuation this light
	void EditAttenuation(const GLuint& shaderID, float constant, float linear, float quadratic, float distanceCutOff);

	// Modifies the direction of this light (Does nothing if we aren't a SPOT/DIRECTIONAL light)
	void EditDirection(const GLuint& shaderID, float x, float y, float z, float w);

	// Modifies the type of light we are
	void EditLightType(const GLuint& shaderID, LightType lightType, float innerAngle, float outerAngle);

	// Modifies if the light is on or off
	void EditState(const GLuint& shaderID, bool on);

	// Will copy this light's information to the shader
	void CopyLightInfoToShader(const GLuint& shaderID);

	const glm::vec4& GetPosition()
	{
		return position;
	}

	const glm::vec4& GetDiffuse()
	{
		return position;
	}

	const glm::vec4& GetSpecular()
	{
		return position;
	}

	const glm::vec4& GetAttenuation()
	{
		return position;
	}

	const glm::vec4& GetDirection()
	{
		return position;
	}

	const LightType& GetLightType()
	{
		return lightType;
	}

	const float& GetInnerAngle()
	{
		return innerAngle;
	}

	const float& GetOuterAngle()
	{
		return outerAngle;
	}

	const bool& GetState()
	{
		return state;
	}

private:
	const int index; // Stores the index the light is at in the fragment shader

	// Lighting members
	glm::vec4 position;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 attenuation;
	glm::vec4 direction;
	LightType lightType;
	float innerAngle;
	float outerAngle;
	bool state;
};

// Adds a light to the scene
Light AddLight(const GLuint& shaderID, float xPos, float yPos, float zPos);