/*
* initShader.h
*
*  Created on: Aug 28, 2017
*      Author: Thumrongsak Kosiyatrakul
*/

#ifndef INITSHADER_H_
#define INITSHADER_H_

#include <glm/glm.hpp>


struct Shader
{
	const char*  filename;
	GLenum       type;
	GLchar*      source;
};

GLuint initShader(const char* vertexShaderFile, const char* fragmentShaderFile);


#endif /* INITSHADER_H_ */