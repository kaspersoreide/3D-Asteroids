#include "loadshaders.h"

GLuint loadShaders(const char* vertex, const char* frag) {
	GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint program = glCreateProgram();
	//Create shaders and shader program
	{
		std::ifstream source_file(vertex);

		std::string data;
		std::getline(source_file, data, '\0');

		const GLchar* vshader_source(data.c_str());

		glShaderSource(vshader, 1, &vshader_source, NULL);
	}
	{
		std::ifstream source_file(frag);

		std::string data;
		std::getline(source_file, data, '\0');

		const GLchar *fshader_source(data.c_str());

		glShaderSource(fshader, 1, &fshader_source, NULL);
	}

	glCompileShader(vshader);
	glCompileShader(fshader);

	glAttachShader(program, vshader);
	glAttachShader(program, fshader);

	glLinkProgram(program);

	glDeleteShader(vshader);
	glDeleteShader(fshader);

	int error = glGetError();

	if (error != GL_NO_ERROR) {
		std::cout << "OpenGL error: " << error << "\n";
	}

	return program;
}

GLuint loadTFBShader(const char* vertex, const GLchar** varyings, int numVaryings) {
	GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
	GLuint program = glCreateProgram();
	//Create shaders and shader program
	{
		std::ifstream source_file(vertex);

		std::string data;
		std::getline(source_file, data, '\0');

		const GLchar* vshader_source(data.c_str());

		glShaderSource(vshader, 1, &vshader_source, NULL);
	}

	glCompileShader(vshader);

	glAttachShader(program, vshader);

	glTransformFeedbackVaryings(program, numVaryings, varyings, GL_INTERLEAVED_ATTRIBS);

	glLinkProgram(program);

	glDeleteShader(vshader);

	int error = glGetError();

	if (error != GL_NO_ERROR) {
		std::cout << "OpenGL error: " << error << "\n";
	}

	return program;
}