#pragma once
#include "GL/glew.h"

class Framebuffer {
public:
	Framebuffer();
	void renderTextureToScreen();
private:
	GLuint texture, depthbuffer, VAO, framebuffer, program;
};
