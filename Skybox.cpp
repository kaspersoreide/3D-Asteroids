#include "Skybox.h"
#include "matrix.h"

GLuint Skybox::program, Skybox::VAO, Skybox::VBO;
int Skybox::N;

void Skybox::Init() {
	program = loadShaders("skyboxVert.shader", "skyboxFrag.shader");
	N = 1000;
	std::vector<float> bufferdata;
	int segmentSize = sizeof(float) * 6;

	glCreateBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	
	for (int i = 0; i < N; i++) {
		vec3 pos = normalize(RNG::randomvec3() - vec3(0.5f));
		bufferdata.push_back(pos[0]);
		bufferdata.push_back(pos[1]);
		bufferdata.push_back(pos[2]);

		vec3 color = vec3(1.0f);
		bufferdata.push_back(color[0]);
		bufferdata.push_back(color[1]);
		bufferdata.push_back(color[2]);
	}
	glBufferData(GL_ARRAY_BUFFER, N * segmentSize, &bufferdata[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,							//position
		3,							//size
		GL_FLOAT,					//type
		GL_FALSE,					//normalize?
		segmentSize,			//stride (in bytes)
		0							//offset
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,							//position
		3,							//size
		GL_FLOAT,					//type
		GL_FALSE,					//normalize?
		segmentSize,			//stride (in bytes)
		(void*)(3 * sizeof(float))	//offset
	);
}

void Skybox::render() {
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glUseProgram(program);
	glBindVertexArray(VAO);
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_POINTS, 0, N);
	glEnable(GL_DEPTH_TEST);
}

void Skybox::setViewProjection(mat3 Rotation, mat4 Projection) {
	mat4 VP = Projection * translateR(transpose(Rotation), vec3(0.0f));
	glUseProgram(program);
	GLuint l_M = glGetUniformLocation(program, "ViewProjection");
	glUniformMatrix4fv(l_M, 1, GL_FALSE, &VP[0][0]);
}

Skybox::Skybox()
{
}


Skybox::~Skybox()
{
}
