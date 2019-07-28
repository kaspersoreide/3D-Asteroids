#pragma once
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <string>

GLuint loadShaders(const char* vertex, const char* frag);

GLuint loadTFBShader(const char* vertex, const GLchar** varyings, int numVaryings);