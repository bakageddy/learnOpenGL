#include "./include/glad/glad.h"
#include <GL/gl.h>
#include "./include/GLFW/glfw3.h"
#include <cstdlib>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <vector>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

using std::string, std::ios, std::vector, std::cout;

std::string load_shader_file(const string& path) {
	std::ifstream file {path.c_str(), ios::ate | ios::binary};
	size_t file_size = file.tellg();
	file.seekg(0, ios::beg);
	vector<char> out(file_size);
	file.read(out.data(), file_size);
	file.close();
	return string(out.data(), file_size);
}

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

	// Data about our triangle
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	unsigned int vbo_id, vao_id;
	// Create a buffer and store it's id in vbo
	glGenBuffers(1, &vbo_id);
	// Create a vertex array object.
	glGenVertexArrays(1, &vao_id);

	// Bind the VAO currently
	glBindVertexArray(vao_id);
	// Bind the VBO to the VAO?
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	// Bind the vertices data to the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Specifies the index and metadata about the data to be sent
	// index, data per vertices, data type, normalize data, stride of data, offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);

	// Specify the vertex attribute index
	glEnableVertexAttribArray(0);

	string vertex_shader_content = load_shader_file("./shaders/triangle.vert.glsl");
	string fragment_shader_content = load_shader_file("./shaders/triangle.frag.glsl");

	const char* vert_shader = vertex_shader_content.c_str();
	const char* frag_shader = fragment_shader_content.c_str();

	unsigned int vertex_shader_id;
	// Create a vertex shader object and compile it
	vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader_id, 1, &vert_shader, NULL);
	glCompileShader(vertex_shader_id);

	cout << "INFO: Compiled Vertex Shader\n";

	unsigned int fragment_shader_id;
	// Create a fragment shader object and compile it
	fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader_id, 1, &frag_shader, NULL);
	glCompileShader(fragment_shader_id);

	cout << "INFO: Compiled Fragment Shader\n";

	unsigned int shader_prog_id;
	// Create a shader program and attach the frag and vert shader
	// and then link it together
	shader_prog_id = glCreateProgram();
	glAttachShader(shader_prog_id, vertex_shader_id);
	glAttachShader(shader_prog_id, fragment_shader_id);
	glLinkProgram(shader_prog_id);

	// Delete it after linking
	glDeleteShader(fragment_shader_id);
	glDeleteShader(vertex_shader_id);


	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// LERPed input to glClearColor
		//			 R     G     B     A
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Use the shader to draw the asset
		glUseProgram(shader_prog_id);

		// Use the vao object
		glBindVertexArray(vao_id);
		// Draw the vao object as arrays
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &vao_id);
	glDeleteBuffers(1, &vbo_id);
	glDeleteProgram(shader_prog_id);

	glfwTerminate();
	return 0;
}
