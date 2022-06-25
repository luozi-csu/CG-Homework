#pragma once
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	std::string vertexString;
	std::string fragmentString;
	const char* vertexSource;
	const char* fragmentSource;
	unsigned int ID;

	void setShaderSource(const char* vertexPath, const char* fragmentPath);
	void use();

private:
	void checkCompileErrors(unsigned int id, std::string type);
};
