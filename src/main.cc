#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <string> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "stb_image.h"
#include "camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const* path);
void GLAPIENTRY opengl_debug_callback(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* user_param) {

	std::cerr << "[OPENGL] debug message: " << message << '\n';
}

const unsigned int window_width = 800;
const unsigned int window_height = 600;

// positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};


float cube_normal_vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

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
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

float cube_vertices[] = {
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
};

float cube_positions_normals_texture_vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

float cube_texture_vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

float position_color_texture_vertices[] = {
	// positions          // colors           // texture coords
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};

float position_color_vertices[] = {
	// positions         // colors
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
};

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


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// timing
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// camera 
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = window_width / 2.0f; // mouse 
float lastY = window_height / 2.0f; // mouse

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main() {
	
	// window stuff.
	GLFWwindow* window = nullptr;

	const float aspect_ratio = static_cast<float>(window_width) / static_cast<float>(window_height);
	// glfw init.
	{
		glfwInit();
		//@NOTE(SJM): we cannot open a debug context on 3.3..? force 4.5.
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		//@NOTE(SJM): we need this window hint, otherwise we cannot get the debug profile.
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
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
			std::cerr << "failed to initialize GlAD.\n";
			return -1;
		}

		// set the window region to the whole screen.
		glViewport(0, 0, window_width, window_height);
		// bind callback when window is resized to also resize viewport
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		// capture the mouse.
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		// bind it to the move callback.
		glfwSetCursorPosCallback(window, mouse_callback);

	}

	// modify global openGL state and retrieve config.
	{
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(opengl_debug_callback, 0);
		int attributes = 0;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attributes);
		std::cerr << "[info] Maximum nr of vertex attributes supported: " << attributes << std::endl;
		int max_uniform_block_size = 0;
		glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &max_uniform_block_size);
		std::cerr << "[info] Max uniform block size (in bytes): " << max_uniform_block_size << std::endl;
		glEnable(GL_DEPTH_TEST);
	}

	Shader lightingShader(
		"C:/Users/sjors/Desktop/learnopengl/project/shaders/multiple_lights.vert",
		"C:/Users/sjors/Desktop/learnopengl/project/shaders/multiple_lights.frag"
	);

	Shader lightCubeShader(
		"C:/Users/sjors/Desktop/learnopengl/project/shaders/light_cube.vert",
		"C:/Users/sjors/Desktop/learnopengl/project/shaders/light_cube.frag"
	);


	// create buffers.
	// our VAO stores our vertex attribute configuration and which VBO to use.
	// the moment we want to draw one of our objects, we take the corresponding VAO, bind it,
	// then draw the object and unbind the VAO again.
	// draw triangle
	unsigned int cubeVAO;
	unsigned int lightCubeVAO;
	unsigned int cubeVBO;

	{
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube_positions_normals_texture_vertices), cube_positions_normals_texture_vertices, GL_STATIC_DRAW);

		glBindVertexArray(cubeVAO);

		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

		// position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// normal.
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glGenVertexArrays(1, &lightCubeVAO);
		glBindVertexArray(lightCubeVAO);

		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		// update this stride because we skip normals.
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

	}

	unsigned int diffuse_texture_id = loadTexture("C:/Users/sjors/Desktop/learnopengl/project/img/textures/container_diffuse.png");
	unsigned int specular_texture_id = loadTexture("C:/Users/sjors/Desktop/learnopengl/project/img/textures/container_specular.png");

	lightingShader.use();
	lightingShader.setInt("material.diffuse", 0); // 0 because it will be bound to GL_TEXTURE0?
	lightingShader.setInt("material.specular", 1);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);
			
		// rendering commands here
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		// since we use a depth buffer, we should also clear that.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.use();
		// set uniforms.
		{
			lightingShader.use();

			lightingShader.setVec3("viewPos", camera.Position);
			lightingShader.setFloat("material.shininess", 64.0f);

			
			// directional light
			lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
			lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
			lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
			lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
			// point light 1
			lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
			lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
			lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
			lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
			lightingShader.setFloat("pointLights[0].constant", 1.0f);
			lightingShader.setFloat("pointLights[0].linear", 0.09f);
			lightingShader.setFloat("pointLights[0].quadratic", 0.032f);
			// point light 2
			lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
			lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
			lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
			lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
			lightingShader.setFloat("pointLights[1].constant", 1.0f);
			lightingShader.setFloat("pointLights[1].linear", 0.09f);
			lightingShader.setFloat("pointLights[1].quadratic", 0.032f);
			// point light 3
			lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
			lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
			lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
			lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
			lightingShader.setFloat("pointLights[2].constant", 1.0f);
			lightingShader.setFloat("pointLights[2].linear", 0.09f);
			lightingShader.setFloat("pointLights[2].quadratic", 0.032f);
			// point light 4
			lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
			lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
			lightingShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
			lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
			lightingShader.setFloat("pointLights[3].constant", 1.0f);
			lightingShader.setFloat("pointLights[3].linear", 0.09f);
			lightingShader.setFloat("pointLights[3].quadratic", 0.032f);
			// spotLight
			lightingShader.setVec3("spotLight.position", camera.Position);
			lightingShader.setVec3("spotLight.direction", camera.Front);
			lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
			lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
			lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
			lightingShader.setFloat("spotLight.constant", 1.0f);
			lightingShader.setFloat("spotLight.linear", 0.09f);
			lightingShader.setFloat("spotLight.quadratic", 0.032f);
			lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
			lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

			// view/projection transformations
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)window_width / (float)window_height, 0.1f, 100.0f);
			glm::mat4 view = camera.GetViewMatrix();
			lightingShader.setMat4("projection", projection);
			lightingShader.setMat4("view", view);


			// update textures.
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuse_texture_id);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, specular_texture_id);

			glBindVertexArray(cubeVAO);
			for (size_t cube_idx = 0; cube_idx != 10; ++cube_idx)
			{
				// world transformation
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, cubePositions[cube_idx]);
				float angle = 20.0f * cube_idx;
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				lightingShader.setMat4("model", model);

				// render the cube.
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			// also draw the lamp object(s)
			lightCubeShader.use();
			lightCubeShader.setMat4("projection", projection);
			lightCubeShader.setMat4("view", view);
			
		}

		// also draw the lamp object(s)
		lightCubeShader.use();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)window_width / (float)window_height, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);

		// we now draw as many light bulbs as we have point lights.
		glBindVertexArray(lightCubeVAO);
		for (unsigned int i = 0; i < 4; i++)
		{
			auto model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			lightCubeShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// check and call events and swap the buffers.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
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

	const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	static bool firstMouse = true;

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	
	camera.ProcessMouseMovement(xoffset, yoffset, GL_TRUE);

}

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}