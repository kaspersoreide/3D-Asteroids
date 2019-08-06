#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 vel;

out vec4 v;

uniform mat4 VP;

void main() {
	gl_Position = VP * vec4(pos, 1.0);
	v = VP * vec4(pos - vel, 1.0);
	//pos2 = VP * vec4(pos + vel, 1.0);
}