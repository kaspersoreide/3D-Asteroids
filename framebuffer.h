#pragma once
#include <GL\glew.h>

GLuint makeFramebufferTexture();

GLuint makeDepthBuffer();

GLuint makeFrameBuffer(GLuint texture, GLuint depthBuffer);

GLuint makeScreenQuadVAO();

void renderTextureToScreen(GLuint vao, GLuint program, GLuint texture);