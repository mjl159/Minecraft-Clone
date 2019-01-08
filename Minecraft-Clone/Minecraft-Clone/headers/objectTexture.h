#ifndef ___OBJECTTEXTURE_H___
#define ___OBJECTTEXTURE_H___

#include <glad/glad.h>
#include <string>

#include "globals.h"
#include "texture.h"

class ObjectTexture
{
public:
	GLuint ID;
	std::string textureKey;

	void generate(std::string textureKey);
	void bind() const;
private:
};

#endif
