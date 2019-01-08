#ifndef ___HELPER_H___
#define ___HELPER_H___

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

#include "texture.h"

class Helper {
public:
	std::map<std::string, Texture> textureMap;

	std::string readFile(const char* path);

	int loadTexture(const char* path, std::string textureKey);
	int updateTexture(const char* path, std::string textureKey);
private:
};

#endif
