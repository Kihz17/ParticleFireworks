#include "TextureManager.h"

#include "SOIL2.H"

GLuint TextureManager::LoadTexture(const char* path)
{
	// Generate a texture ID for ourselves
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	uint8_t* data = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB); // Read image file

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