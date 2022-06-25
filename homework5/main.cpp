#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Camera.h"

float vertices[] = {
	// positions          // normals
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,	1.0f, 
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,	1.0f, 
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,	1.0f, 
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,	1.0f, 
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,	1.0f, 
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,	1.0f, 

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
};

// create camera
Camera camera(glm::vec3(0, 0, 3.0f), glm::radians(0.0f), glm::radians(0.0f), glm::vec3(0, 1.0f, 0));

float lastX, lastY;
bool firstMouse = true;
void processInput(GLFWwindow* window, Shader* shader)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	// use w, s, a, d to move camera
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.speedZ = -0.1f;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.speedZ = 0.1f;
	}
	else {
		camera.speedZ = 0.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.speedX = 0.1f;
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.speedX = -0.1f;
	}
	else {
		camera.speedX = 0.0f;
	}

	// use q, e to switch light model
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		shader->setShaderSource("phongVertex.vert", "phongFragment.frag");
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		shader->setShaderSource("gouraudVertex.vert", "gouraudFragment.frag");
	}
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse) {
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}
	float deltax, deltay;
	deltax = xPos - lastX;
	deltay = yPos - lastY;
	lastX = xPos;
	lastY = yPos;
	camera.processMousemovement(deltax, deltay);
}

int main()
{
	// glfw init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "homework5", NULL, NULL);

	if (window == NULL) {
		std::cout << "create window failed" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// disable cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// enalbe mouse callback
	glfwSetCursorPosCallback(window, mouse_callback);

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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// create shader
	Shader* shader = new Shader("phongVertex.vert", "phongFragment.frag");

	// opengl status
	glEnable(GL_DEPTH_TEST);

	// view & transform & projection matrix
	glm::mat4 viewMatrix(1.0f);
	viewMatrix = glm::translate(viewMatrix, glm::vec3(0, 0, -3.0f));
	glm::mat4 transformMatrix(1.0f);
	transformMatrix = glm::rotate(transformMatrix, glm::radians(60.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.f);

	glm::mat4 modelMatrix(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -2.0f));

	while (!glfwWindowShouldClose(window)) {
		// process input
		processInput(window, shader);

		// clear screen
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// use shader
		shader->use();

		// bind vao
		glBindVertexArray(VAO);

		// get view matrix from camera
		viewMatrix = camera.GetLookAtMatrix();

		// set view & transform & projection matrix
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "transform"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

		// set color
		glUniform3f(glGetUniformLocation(shader->ID, "objectColor"), 0.8f, 0.3f, 0.2f);
		glUniform3f(glGetUniformLocation(shader->ID, "lightColor"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(shader->ID, "ligthPos"), 1.2f, 1.0f, 2.0f);
		glUniform3f(glGetUniformLocation(shader->ID, "viewPos"), camera.position.x, camera.position.y, camera.position.z);

		// draw cube
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwPollEvents();
		camera.updateCameraPosition();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}