#version 330 core

layout(location = 0) in vec2 pos;

out vec2 screenPos;

void main() {
	gl_Position = vec4(pos, 0.0, 1.0);
	screenPos = pos;
}