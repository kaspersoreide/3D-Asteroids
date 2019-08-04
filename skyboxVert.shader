#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;

out vec3 theColor;

uniform mat4 ViewProjection;

void main() {
	vec4 sp = ViewProjection * vec4(99.5 * pos, 1.0);
	//sp[3] = 0.5;
	gl_Position = sp;
	theColor = vec3(1.0, 1.0, 1.0);
}