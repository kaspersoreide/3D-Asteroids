#version 330 core

layout(points) in;
layout(line_strip, max_vertices = 2) out;

in vec4 v[];

out vec3 p;

uniform mat4 VP;

void main() {
	p = gl_in[0].gl_Position.xyz;
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	//gl_Position = pos2[0];
	p = v[0].xyz;
	gl_Position = v[0];
	//gl_Position = VP * vec4(0.0, 0.0, 0.0, 1.0);
	EmitVertex();
	EndPrimitive();
}