#include <iostream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float flagVertices[] = {
	// positions           // textrue coordinates
	-0.75f, -0.5f,  0.0f,  0.0f,  0.0f,
	 0.75f, -0.5f,  0.0f,  1.0f,  0.0f,
	-0.75f,  0.5f,  0.0f,  0.0f,  1.0f,
	 0.75f,  0.5f,  0.0f,  1.0f,  1.0f,
};

float vertices[] = {
	// positions            // textrue coordinates
	-0.75f, -0.75f,  0.0f,  0.0f,  0.0f,
	 0.75f, -0.75f,  0.0f,  3.0f,  0.0f,
	-0.75f,  0.75f,  0.0f,  0.0f,  3.0f,
	 0.75f,  0.75f,  0.0f,  3.0f,  3.0f,
};

float cameraZ = -3.0f;

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraZ += (float)glfwGetTime() * 0.02f;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraZ -= (float)glfwGetTime() * 0.02f;
	}
}

unsigned int loadImage(const char* filepath, GLint internalformat, GLenum format)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // use repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // use repeat
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); // use clamp
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); // use clamp

	// texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// texture mipmap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// use anisotropic filtering
	// af parameter
	float afLargest;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &afLargest);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, afLargest);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return texture;
}

int main()
{
	// glfw init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1600, 1200, "homework1", NULL, NULL);

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
	glViewport(0, 0, 1600, 1200);

	// load model to VAO & VBO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(flagVertices), flagVertices, GL_STATIC_DRAW); // draw national flag
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // draw jerry

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// init shader
	Shader* shader = new Shader("vertexSource.vert", "fragmentSource.frag");

	// load image and bind texture
	stbi_set_flip_vertically_on_load(true);
	unsigned int flagTexture = loadImage("flag.png", GL_RGB, GL_RGB);
	unsigned int jerryTexture = loadImage("jerry.jpg", GL_RGB, GL_RGB);

	// projection matrix
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	while (!glfwWindowShouldClose(window)) {
		// process input
		processInput(window);

		// clear screen
		glClearColor(0.3f, 0.2f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->use();

		glBindVertexArray(VAO);

		// activate and use texture0
		glUniform1i(glGetUniformLocation(shader->ID, "flagTexture"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, flagTexture);

		// activate and use texture1
		glUniform1i(glGetUniformLocation(shader->ID, "jerryTexture"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, jerryTexture);

		// create and process view matrix
		glm::mat4 viewMatrix(1.0f);
		viewMatrix = glm::translate(viewMatrix, glm::vec3(0, 0, cameraZ));

		// set view matrix and projection matrix to shader program
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}