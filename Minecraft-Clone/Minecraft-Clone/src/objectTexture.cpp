#include "../headers/objectTexture.h"

void ObjectTexture::generate(std::string textureKey)
{
	this->textureKey = textureKey;

	Texture texture = helper.textureMap[textureKey];

	// Generate texture
	glGenTextures(1, &this->ID);

	// Bind texture
	glBindTexture(GL_TEXTURE_2D, this->ID);

	// Set texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ObjectTexture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}