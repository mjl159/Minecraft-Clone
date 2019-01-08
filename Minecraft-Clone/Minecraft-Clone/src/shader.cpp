#include "../headers/shader.h"

/*
  _____         _      _  _
 |  __ \       | |    | |(_)
 | |__) |_   _ | |__  | | _   ___
 |  ___/| | | || '_ \ | || | / __|
 | |    | |_| || |_) || || || (__
 |_|     \__,_||_.__/ |_||_| \___|

*/
Shader::Shader() {

}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vShaderString = helper.readFile(vertexPath);
	std::string fShaderString = helper.readFile(fragmentPath);

	const char* vShaderCode = vShaderString.c_str();
	const char* fShaderCode = fShaderString.c_str();

	// COMPILE SHADERS
	GLuint vertex, fragment;

	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use(void) {
	glUseProgram(ID);
}

/*
  _____        _               _
 |  __ \      (_)             | |
 | |__) |_ __  _ __   __ __ _ | |_  ___
 |  ___/| '__|| |\ \ / // _` || __|/ _ \
 | |    | |   | | \ V /| (_| || |_|  __/
 |_|    |_|   |_|  \_/  \__,_| \__|\___|

*/
void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else 
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}