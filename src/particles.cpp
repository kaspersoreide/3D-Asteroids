#include "particles.h"


GLuint ParticleCluster::renderProgram;
GLuint ParticleCluster::transformProgram;
vector<ParticleCluster*> ParticleCluster::particles;

void ParticleCluster::loadPrograms() {
	renderProgram = loadShaders(
		"shaders/particles/vert.glsl",
		"shaders/particles/frag.glsl"
	);
	const GLchar* varyings[] = { "outPos1", "outVel1", "outPos2", "outVel2" };
	transformProgram = loadTFBShader(
		"shaders/particles/tfb.glsl",
		varyings,
		4
	);
}

void ParticleCluster::setViewProjection(mat4 VP) {
	glUseProgram(renderProgram);
	GLuint l_M = glGetUniformLocation(renderProgram, "ViewProjection");
	glUniformMatrix4fv(l_M, 1, GL_FALSE, &VP[0][0]);
}

ParticleCluster::ParticleCluster(int n, vec3 origin, vec3 color, float radius) : 
	origin(origin), color(color), radius(radius) {

	particleCount = n;
	std::vector<float> bufferdata(6 * n);
	int nicolasCage = sizeof(float) * 6;
	t = 0;

	glGenVertexArrays(1, &VAOr);
	glBindVertexArray(VAOr);

	glCreateBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	for (int i = 0; i < n; i++) {
		float f = RNG::randomFloat();
		bufferdata[6 * i + 3] = f;
	}
	glBufferData(GL_ARRAY_BUFFER, n * nicolasCage, &bufferdata[0], GL_DYNAMIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, n * nicolasCage, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,							//position
		3,							//size
		GL_FLOAT,					//type
		GL_FALSE,					//normalize?
		nicolasCage,			//stride (in bytes)
		0							//offset
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,							//position
		3,							//size
		GL_FLOAT,					//type
		GL_FALSE,					//normalize?
		nicolasCage,			//stride (in bytes)
		(void*)(3 * sizeof(float))	//offset
	);

	glGenVertexArrays(1, &VAOt);
	glBindVertexArray(VAOt);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,							//position
		3,							//size
		GL_FLOAT,					//type
		GL_FALSE,					//normalize?
		2 * nicolasCage,			//stride (in bytes)
		0							//offset
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,							//position
		3,							//size
		GL_FLOAT,					//type
		GL_FALSE,					//normalize?
		2 * nicolasCage,			//stride (in bytes)
		(void*)(3 * sizeof(float))	//offset
	);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,							//position
		3,							//size
		GL_FLOAT,					//type
		GL_FALSE,					//normalize?
		2 * nicolasCage,			//stride (in bytes)
		(void*)(6 * sizeof(float))	//offset
	);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(
		3,							//position
		3,							//size
		GL_FLOAT,					//type
		GL_FALSE,					//normalize?
		2 * nicolasCage,			//stride (in bytes)
		(void*)(9 * sizeof(float))	//offset
	);

	glGenBuffers(1, &TBO);
	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, n * nicolasCage, NULL, GL_DYNAMIC_DRAW);
}


void ParticleCluster::doStuff() {
	++t;
	if (t > 600) {
		particles.erase(find(particles.begin(), particles.end(), this));
		delete this;
		return;
	}
	setUniforms();
	glUseProgram(transformProgram);
	glBindVertexArray(VAOt);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glEnable(GL_RASTERIZER_DISCARD);
	//glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, TBO);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, TBO);
	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, particleCount);
	glEndTransformFeedback();
	glUseProgram(renderProgram);
	glBindVertexArray(VAOr);
	glDisable(GL_RASTERIZER_DISCARD);
	glDrawArrays(GL_LINES, 0, particleCount);

	GLuint temp = TBO;
	TBO = VBO;
	VBO = temp;
}

void ParticleCluster::setUniforms() {
	glUseProgram(transformProgram);
	GLuint l_o = glGetUniformLocation(transformProgram, "origin");
	glUniform3fv(l_o, 1, &origin[0]);
	GLuint l_r = glGetUniformLocation(transformProgram, "radius");
	glUniform1fv(l_r, 1, &radius);
	glUseProgram(renderProgram);
	GLuint l_c = glGetUniformLocation(renderProgram, "color");
	glUniform3fv(l_c, 1, &color[0]);
}

ParticleCluster::~ParticleCluster() {
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &TBO);
	glDeleteVertexArrays(1, &VAOr);
	glDeleteVertexArrays(1, &VAOt);
}