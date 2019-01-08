#include "../headers/helper.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

std::string Helper::readFile(const char* path) {
	std::string contents;
	std::ifstream file;

	// ensure ifstream objects can throw exceptions:
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		file.open(path);
		std::stringstream stream;
		// read file's buffer contents into streams
		stream << file.rdbuf();
		// close file handlers
		file.close();
		// convert stream into string
		contents = stream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		std::cout << "ERROR: " << strerror(errno) << std::endl;
	}

	return contents;
}

int Helper::loadTexture(const char* path, std::string textureKey) {
	if (textureMap.count(textureKey)) {
		std::cout << textureKey << " already exists." << std::endl;
		return 0;
	}
	else {
		Texture newTexture = Texture(path, textureKey);
		unsigned char *data = stbi_load(path, &newTexture.width, &newTexture.height, &newTexture.nrChannels, 0);
		if (data)
		{
			newTexture.data = data;
			textureMap[textureKey] = newTexture;
			return 1;
		}
		else
		{
			std::cout << "Failed to load texture: " << textureKey << std::endl;
			return 0;
		}
	}
}

int Helper::updateTexture(const char* path, std::string textureKey) {
	if (textureMap.count(textureKey)) {
		Texture updatedTexture = Texture(path, textureKey);
		unsigned char *data = stbi_load(path, &updatedTexture.width, &updatedTexture.height, &updatedTexture.nrChannels, 0);
		if (data)
		{
			updatedTexture.data = data;
			textureMap[textureKey] = updatedTexture;
			return 1;
		}
		else
		{
			std::cout << "Failed to load texture: " << textureKey << std::endl;
			return 0;
		}
	}
	else {
		std::cout << "Failed to find texture: " << textureKey << std::endl;
		return 0;
	}
}
