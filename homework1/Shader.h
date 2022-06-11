#pragma once
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Shader
{
private:
	GLenum shaderType;
	std::string fileSourceString;
	const char* fileSource;
	unsigned int ID;

	void checkShaderCompile();
	void checkProgramLink();
public:
	Shader(GLenum type, const char* filePath);
	void use();
};
