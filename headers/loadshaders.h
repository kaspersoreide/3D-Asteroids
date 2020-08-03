#pragma once
#include "GL/glew.h"
#include <iostream>
#include <fstream>
#include <string>

GLuint loadShaders(const char* vertex, const char* frag);

GLuint loadTFBShader(const char* vertex, const GLchar** varyings, int numVaryings);

GLuint loadGeometryShader(const char* vertex, const char* geo, const char* frag);