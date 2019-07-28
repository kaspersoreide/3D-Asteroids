#version 330 core

layout(location = 0) in vec3 pos;

uniform mat4 Model;
uniform mat4 VP;
uniform vec3 pColor;

out vec4 color;

void main() {
	//gl_Position = vec4(0.25 * pos + 0.25, 1.0);
	//vec3 n = (Model * vec4(normal, 0.0)).xyz;
	//light = clamp(dot(n, vec3(-0.8, 0.2, 0.0)), 0.1, 1.0);
	vec4 sPos = VP * Model * vec4(pos, 1.0);
	gl_Position = sPos;
	color = vec4(pColor - 0.2 * pos, 1.0) - 0.006 * sPos.z;//vec4(abs(pos) * pColor, 1.0);
}