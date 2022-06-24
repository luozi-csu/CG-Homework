#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

bool resetSwitch = true;

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

void processInput(GLFWwindow* window, Shader* shader)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	// translation
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		resetSwitch = false;
		glm::mat4 translationMatrix(1.0f);
		translationMatrix = glm::translate(translationMatrix, glm::vec3(0.2f, 0.3f, 0));
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "transform"), 1, GL_FALSE, glm::value_ptr(translationMatrix));
	}
	// rotate
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		resetSwitch = false;
		glm::mat4 rotateMatrix(1.0f);
		rotateMatrix = glm::rotate(rotateMatrix, (float)glfwGetTime(), glm::vec3(0, 0, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "transform"), 1, GL_FALSE, glm::value_ptr(rotateMatrix));
	}
	// scale
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		resetSwitch = false;
		glm::mat4 scaleMatrix(1.0f);
		scaleMatrix = glm::scale(scaleMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "transform"), 1, GL_FALSE, glm::value_ptr(scaleMatrix));
	}
	// rotate by point (0.3, 0.3, 0)
	else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		resetSwitch = false;
		glm::mat4 transMatrix(1.0f);
		transMatrix = glm::translate(transMatrix, glm::vec3(-0.3f, -0.3f, 0));
		glm::mat4 rotateMatrix(1.0f);
		rotateMatrix = glm::rotate(rotateMatrix, (float)glfwGetTime(), glm::vec3(0, 0, 1.0f));
		glm::mat4 invTransMatrix(1.0f);
		invTransMatrix = glm::translate(invTransMatrix, glm::vec3(0.3f, 0.3f, 0));
		glm::mat4 combineMatrix = invTransMatrix * rotateMatrix * transMatrix;
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "transform"), 1, GL_FALSE, glm::value_ptr(combineMatrix));
	}
	// scale by point (0.5, 0.5, 0)
	else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		resetSwitch = false;
		glm::mat4 transMatrix(1.0f);
		transMatrix = glm::translate(transMatrix, glm::vec3(-0.5f, -0.5f, 0));
		glm::mat4 scaleMatrix(1.0f);
		scaleMatrix = glm::scale(scaleMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
		glm::mat4 invTransMatrix(1.0f);
		invTransMatrix = glm::translate(invTransMatrix, glm::vec3(0.5f, 0.5f, 0));
		glm::mat4 combineMatrix = invTransMatrix * scaleMatrix * transMatrix;
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "transform"), 1, GL_FALSE, glm::value_ptr(combineMatrix));
	}
	// combination of translation and rotate
	else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		resetSwitch = false;
		glm::mat4 translationMatrix(1.0f);
		translationMatrix = glm::translate(translationMatrix, glm::vec3(0.5f, 0.5f, 0));
		glm::mat4 rotateMatrix(1.0f);
		rotateMatrix = glm::rotate(rotateMatrix, glm::radians(30.0f), glm::vec3(0, 0, 1.0f));
		glm::mat4 combineMatrix = rotateMatrix * translationMatrix;
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "transform"), 1, GL_FALSE, glm::value_ptr(combineMatrix));
	}
	// shear
	else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		resetSwitch = false;
		glm::mat4 shearMatrix(1.0f);
		shearMatrix[1][0] = 1.0f;
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "transform"), 1, GL_FALSE, glm::value_ptr(shearMatrix));
	}
	// reflect
	else if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
		resetSwitch = false;
		glm::mat4 reflectMatrix(1.0f);
		reflectMatrix[1][1] = -1.0f;
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "transform"), 1, GL_FALSE, glm::value_ptr(reflectMatrix));
	}
	// reset
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		resetSwitch = true;
	}
}

int main()
{
	// glfw init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "homework2", NULL, NULL);

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

	// init shader
	Shader* shader = new Shader("vertexSource.vert", "fragmentSource.frag");

	while (!glfwWindowShouldClose(window)) {

		if (resetSwitch) {
			glUniformMatrix4fv(glGetUniformLocation(shader->ID, "transform"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
		}

		// process input
		processInput(window, shader);

		// clear screen
		glClearColor(0.3f, 0.2f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// use shader program
		shader->use();

		glBindVertexArray(VAO);
		// draw triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}