#include "shader.h"

namespace {
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
		return std::move(buffer.str());
	}

}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexShaderString = file_to_string(vertexPath);
	std::string fragmentShaderString = file_to_string(fragmentPath);
	const char* vertexShaderSource = vertexShaderString.data();
	const char* fragmentShaderSource = fragmentShaderString.data();

	int  success;
	constexpr const int infoLogLength = 512;
	char infoLog[infoLogLength];


	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	{
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		// was shader compilation successful?
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, infoLogLength, NULL, infoLog);
			std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	{
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		// was shader compilation successful?

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, infoLogLength, NULL, infoLog);
			std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	ID = 0;
	{
		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ID, infoLogLength, NULL, infoLog);
			std::cerr << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}
	// these shaders are bound to the shader program and we are free to delete them
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}