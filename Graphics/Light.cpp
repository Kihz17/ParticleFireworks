#include "Light.h"

#include <iostream>
#include <sstream>

static unsigned int currentLightIndex = 0;
const unsigned int maxLights = 10;

Light::Light(int index) :
	index(index), 
	position(0.0f, 0.0f, 0.0f, 0.0f), 
	diffuse(1.0f, 1.0f, 1.0f, 1.0f),
	specular(1.0f, 1.0f, 1.0f, 1.0f),
	attenuation(0.0f, 0.1f, 0.01f, 100000.0f),
	direction(0.0f, -1.0f, 0.0f, 0.0f),
	innerAngle(0.0f),
	outerAngle(0.0f),
	state(true)
{

}

Light::~Light()
{

}

void Light::EditPosition(const GLuint& shaderID, float x, float y, float z, float w)
{
	this->position = glm::vec4(x, y, z, w);
}

void Light::EditDiffuse(const GLuint& shaderID, float x, float y, float z, float w)
{
	this->diffuse = glm::vec4(x, y, z, w);
}

void Light::EditSpecular(const GLuint& shaderID, float r, float g, float b, float power)
{
	this->specular = glm::vec4(r, g, b, power);
}

void Light::EditAttenuation(const GLuint& shaderID, float constant, float linear, float quadratic, float distanceCutOff)
{
	this->attenuation = glm::vec4(constant, linear, quadratic, distanceCutOff);
}

void Light::EditDirection(const GLuint& shaderID, float x, float y, float z, float w)
{
	this->direction = glm::vec4(x, y, z, w);
}

void Light::EditLightType(const GLuint& shaderID, LightType lightType, float innerAngle, float outerAngle)
{
	this->lightType = lightType;
	this->innerAngle = innerAngle;
	this->outerAngle = outerAngle;
}

void Light::EditState(const GLuint& shaderID, bool on)
{
	this->state = on;
}

void Light::CopyLightInfoToShader(const GLuint& shaderID)
{
	std::stringstream ss;
	ss << "theLights[" << this->index << "].position";
	glUniform4f(glGetUniformLocation(shaderID, ss.str().c_str()), this->position.x, this->position.y, this->position.z, this->position.w);
	ss.str("");

	ss << "theLights[" << this->index << "].diffuse";
	glUniform4f(glGetUniformLocation(shaderID, ss.str().c_str()), this->diffuse.x, this->diffuse.y, this->diffuse.z, this->diffuse.w);
	ss.str("");

	ss << "theLights[" << this->index << "].specular";
	glUniform4f(glGetUniformLocation(shaderID, ss.str().c_str()), this->specular.x, this->specular.y, this->specular.z, this->specular.w);
	ss.str("");

	ss << "theLights[" << this->index << "].atten";
	glUniform4f(glGetUniformLocation(shaderID, ss.str().c_str()), this->attenuation.x, this->attenuation.y, this->attenuation.z, this->attenuation.w);
	ss.str("");

	ss << "theLights[" << this->index << "].direction";
	glUniform4f(glGetUniformLocation(shaderID, ss.str().c_str()), this->direction.x, this->direction.y, this->direction.z, this->direction.w);
	ss.str("");

	ss << "theLights[" << this->index << "].param1";
	glUniform4f(glGetUniformLocation(shaderID, ss.str().c_str()), (float)this->lightType, this->innerAngle, this->outerAngle, 1.0f);
	ss.str("");

	ss << "theLights[" << this->index << "].param2";
	if (this->state)
	{
		glUniform4f(glGetUniformLocation(shaderID, ss.str().c_str()), (float) GL_TRUE, 1.0f, 1.0f, 1.0f);
	}
	else
	{
		glUniform4f(glGetUniformLocation(shaderID, ss.str().c_str()), (float) GL_FALSE, 1.0f, 1.0f, 1.0f);
	}

	ss.str("");
}

Light AddLight(const GLuint& shaderID, float xPos, float yPos, float zPos)
{
	if (currentLightIndex >= maxLights)
	{
		std::cout << "We have reach the maximum light limit (" << maxLights << "). To add more lights, you must increase the number of lights in the fragment shader and Light.cpp" << std::endl;
		return NULL;
	}

	Light light(currentLightIndex++);
	light.EditPosition(shaderID, xPos, yPos, zPos, 1.0f);
	return light;
}