#include "../headers/texture.h"

Texture::Texture() {

}

Texture::Texture(const char* path, std::string textureKey) {
	this->path = path;
	this->textureKey = textureKey;
}