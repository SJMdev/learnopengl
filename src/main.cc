#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <string> 
#include <fstream> // file_to_string
#include <sstream> // file_to_string


std::string file_to_string(const std::string& path);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f , 0.0f
};

float quad_vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top 
};

unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

int main() {
	
	std::cerr << "hello world!\n" << '\n';

	// window stuff.
	GLFWwindow* window = nullptr;
	const int window_width = 800;
	const int window_height = 600;
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
		window = glfwCreateWindow(window_width, window_height, "LearnOpenGL", NULL, NULL);
		if (window == nullptr) {
			std::cerr << "failed to create window" << '\n';
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "failed to initialize GlAD.\n";
			return -1;
		}

		// set the window region to the whole screen.
		glViewport(0, 0, window_width, window_height);
		// bind callback when window is resized to also resize viewport
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	}


	// openGL info.
	{
		int nrAttributes;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
		std::cerr << "[info] Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
	}


	std::string vertexShaderString = file_to_string("C:/Users/sjors/Desktop/learnopengl/project/shaders/passthrough.vert");
	std::string fragmentShaderString = file_to_string("C:/Users/sjors/Desktop/learnopengl/project/shaders/passthrough.frag");
	const char* vertexShaderSource = vertexShaderString.data();
	const char* fragmentShaderSource = fragmentShaderString.data();

	// create vertex shader.
	unsigned int vertexShader;
	{
		vertexShader = glCreateShader(GL_VERTEX_SHADER);

		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		// was shader compilation successful?
		int  success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	// create fragment shader
	unsigned int fragmentShader;
	{
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		
		// was shader compilation successful?
		int  success;
		char infoLog[512];
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	// create shader program.
	unsigned int shaderProgram;
	{
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		int  success;
		char infoLog[512];
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}
	// these shaders are bound to the shader program and we are free to delete them
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	// create buffers.
	// our VAO stores our vertex attribute configuration and which VBO to use.
	// the moment we want to draw one of our objects, we take the corresponding VAO, bind it,
	// then draw the object and unbind the VAO again.
	// draw triangle
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	//glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);




	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);
			
		// rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		// update uniform color.
		{
			// set uniforms
			float timeValue = glfwGetTime();
			float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
			int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
			glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		}

		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// check and call events and swap the buffers.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


// https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
std::string file_to_string(const std::string& path) {
	// Open the file
	std::ifstream file(path);
	if (!file.is_open()) {
		std::cerr << "[error] could not open file " << path << '\n';
	}

	// Read the contents of the file into a stringstream
	std::stringstream buffer;
	buffer << file.rdbuf();

	// Return the contents of the stringstream as a string
	return buffer.str();
}


void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		static bool use_wireframe = false;

		use_wireframe = !use_wireframe;
		
		if (use_wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			return;
		}
		else { 
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			return; 
		}
		
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}