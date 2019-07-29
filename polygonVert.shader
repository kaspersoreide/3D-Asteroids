#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

uniform mat4 Model;
uniform mat4 VP;
uniform vec3 pColor;

out vec4 color;

const vec3 light = vec3(1.0, 0.0, 0.0);

void main() {
	//gl_Position = vec4(0.25 * pos + 0.25, 1.0);
	//vec3 n = (Model * vec4(normal, 0.0)).xyz;
	//light = clamp(dot(n, vec3(-0.8, 0.2, 0.0)), 0.1, 1.0);
	vec4 sPos = VP * Model * vec4(pos, 1.0);
	gl_Position = sPos;
	vec3 rNormal = vec3(Model * vec4(normal, 0.0));
	float lit = clamp(dot(rNormal, light), 0.3, 1.0);
	color = vec4(lit * pColor, 1.0);// - 0.006 * sPos.z;//vec4(abs(pos) * pColor, 1.0);
}