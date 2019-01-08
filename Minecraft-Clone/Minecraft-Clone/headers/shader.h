#ifndef ___SHADER_H___
#define ___SHADER_H___

#include <glad/glad.h>

#include "helper.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	GLuint ID;
	Shader();
	Shader(const char* vertexPath, const char* fragmentPath);
	void use(void);
private:
	Helper helper;
	void checkCompileErrors(unsigned int shader, std::string type);
};

#endif