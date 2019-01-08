#ifndef ___TEXTURE_H___
#define ___TEXTURE_H___

#include <string>
#include <iostream>

class Texture
{
public:
	std::string textureKey;
	const char* path;
	int width;
	int height;
	int nrChannels;
	unsigned char* data;

	Texture();
	Texture(const char* path, std::string textureKey);

private:
};
#endif