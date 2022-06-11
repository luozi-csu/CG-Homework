#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

//#define PURE_COLOR
#define GRADIENT

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main()
{
	// glfw init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "homework1", NULL, NULL);

	if (window == NULL) {
		std::cout << "create window failed" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// disable cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// init glew
	glewExperimental = true;

	if (glewInit() != GLEW_OK) {
		std::cout << "glew init failed" << std::endl;
		glfwTerminate();
		return -1;
	}

	// show window
	glViewport(0, 0, 800, 600);

	// load model to VAO & VBO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	Shader* vertexShader = new Shader(GL_VERTEX_SHADER, "vertexSource.vert");
	Shader* fragmentShader = NULL;

	while (!glfwWindowShouldClose(window)) {
		// process input
		processInput(window);

		// clear screen
		glClearColor(0.3f, 0.2f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef PURE_COLOR
		fragmentShader = new Shader(GL_FRAGMENT_SHADER, "pure_color.frag");
		fragmentShader->use();
		fragmentShader->deleteShader();
#endif

#ifdef GRADIENT
		fragmentShader = new Shader(GL_FRAGMENT_SHADER, "gradient.frag");
		unsigned int program = glCreateProgram();
		glAttachShader(program, vertexShader->shaderID);
		glAttachShader(program, fragmentShader->shaderID);
		glLinkProgram(program);

		// check program link status
		int linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);

		if (!linked) {
			std::cout << "link program failed" << std::endl;
			return -1;
		}

		glUseProgram(program);
		fragmentShader->deleteShader();
#endif

		glBindVertexArray(VAO);
		// draw triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwPollEvents();
		glfwSwapBuffers(window);

		// deallocate memory
		glDeleteProgram(program);
		delete fragmentShader;
	}

	glfwTerminate();
	return 0;
}