#include <iostream>

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

//#define DRAW_VERTICES
#define DRAW_CUBE
//#define ORTHO
#define PERSPECTIVE

float vertices[] = {
	// position
	-0.5f,     0.0f, -0.2887f,
	 0.5f,     0.0f, -0.2887f,
	 0.0f,     0.0f,  0.5774f,
	 0.0f,  0.8165f,     0.0f,
	-0.5f,     0.0f, -0.2887f,
	 0.5f,     0.0f, -0.2887f,
};

float cubePositions[] = {
	// position
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f,  0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
};

unsigned int cubeIndices[] = {
	0, 1, 2, 3, 6, 7, 4, 5,
	0xffff,
	2, 6, 0, 4, 1, 5, 3, 7
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

	GLFWwindow* window = glfwCreateWindow(800, 600, "homework3", NULL, NULL);

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

#if defined(DRAW_VERTICES)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
#elif defined(DRAW_CUBE)
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubePositions), cubePositions, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);
#endif

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// init shader
	Shader* shader = new Shader("vertexSource.vert", "fragmentSource.frag");

	// opengl status
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

#if defined(DRAW_CUBE)
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xffff);
#endif

	glm::mat4 viewMatrix(1.0f);
	glm::mat4 projectionMatrix(1.0f);
	viewMatrix = glm::translate(viewMatrix, glm::vec3(0, 0, -3.0f));
#if defined(ORTHO)
	projectionMatrix = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
#elif defined(PERSPECTIVE)
	projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.f);
#endif

	while (!glfwWindowShouldClose(window)) {
		// process input
		processInput(window);

		// clear screen
		glClearColor(0.3f, 0.2f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 transformMatrix(1.0f);
		transformMatrix = glm::rotate(transformMatrix, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "transform"), 1, GL_FALSE, glm::value_ptr(transformMatrix));
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		shader->use();

		glBindVertexArray(VAO);

#if defined(DRAW_VERTICES)
		// draw tetrahedron
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
#elif defined(DRAW_CUBE)
		// draw cube
		glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_INT, NULL);
#endif
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}