#pragma once
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Shader
{
public:
	GLenum shaderType;
	std::string fileSourceString;
	const char* fileSource;
	unsigned int shaderID;
	unsigned int ID;

	Shader(GLenum type, const char* filePath);
	void deleteShader();
};
