#include "./include/glad/glad.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

void frameBufferCallBack(GLFWwindow* w, int width, int height) {
	// Since all the coords in OpenGL are normalized to -1.0f to 1.0f
	// We need to convert it to coordinates from the view port
	// idk what a view port is but i have accepted it's existence
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main(void) {

	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "I am learning opengl", NULL, NULL);
	if (!window) {
		std::cerr << "ERR: Failed to create window!\n";
		std::cerr << "ERR: " << glfwGetError(NULL);
		glfwTerminate();
		std::exit(-1);
	}

	glfwSetFramebufferSizeCallback(window, frameBufferCallBack);

	std::cout << "INFO: Created window!\n";

	glfwMakeContextCurrent(window);

	// Remember to load glad after making context current
	// Init GLAD
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::cerr << "ERR: Failed to load GLAD\n";
		std::exit(-1);
	}

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	unsigned int vbo_id;
	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// LERPed input to glClearColor
		//			 R     G     B     A
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
