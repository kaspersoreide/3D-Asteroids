#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 vel;

out vec3 theColor;

uniform mat4 ViewProjection;
uniform vec3 color;

void main() {
	vec4 sp = ViewProjection * vec4(pos, 1.0);
	gl_Position = sp;
	theColor = color;
}