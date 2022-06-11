#include "Shader.h"

Shader::Shader(GLenum type, const char* filePath)
{
	std::ifstream infileStream;
	std::stringstream infileString;

	infileStream.open(filePath);

	try
	{
		if (type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER) {
			throw std::exception("unknown shader type");
		}

		// load shader type
		shaderType = type;

		if (!infileStream.is_open()) {
			throw std::exception("open file failed");
		}

		// read file stream buffer to string stream
		infileString << infileStream.rdbuf();
		// load shader file source string
		fileSourceString = infileString.str();
		// load shader file source, std::string to const char pointer
		fileSource = fileSourceString.c_str();

		shaderID = glCreateShader(shaderType);
		glShaderSource(shaderID, 1, &fileSource, NULL);
		glCompileShader(shaderID);
		
		// check shader compile status
		int compiled;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);

		if (!compiled) {
			throw std::exception("compile shader failed");
		}

		ID = glCreateProgram();
		glAttachShader(ID, shaderID);
		glLinkProgram(ID);

		// check program link status
		int linked;
		glGetProgramiv(ID, GL_LINK_STATUS, &linked);

		if (!linked) {
			throw std::exception("link program failed");
		}
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what();
	}
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::deleteShader()
{
	glDeleteShader(shaderID);
}
