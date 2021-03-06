#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

uniform mat4 Model;
uniform mat4 VP;
uniform vec3 cPos;
uniform vec3 pColor;
uniform bool outline;

out Data {
	vec3 vNormal;
	vec3 rPos;
	vec3 center;
};
//const vec3 light = vec3(1.0, 0.0, 0.0);

void main() {
	mat4 MVP = VP * Model;
	//gl_Position = vec4(0.25 * pos + 0.25, 1.0);
	//vec3 n = (Model * vec4(normal, 0.0)).xyz;
	//light = clamp(dot(n, vec3(-0.8, 0.2, 0.0)), 0.1, 1.0);
	if (outline) {
		gl_Position = MVP * vec4(1.01 * pos, 1.0);
	}
	else {
		gl_Position = MVP * vec4(pos, 1.0);
	}
	vNormal = normalize(vec3(Model * vec4(normal, 0.0)));
	rPos = vec3(Model * vec4(pos, 1.0));
	center = vec3(Model * vec4(0.0, 0.0, 0.0, 1.0));
	/*screenPos = sPos.xyz;
	center = vec3(VP * Model * vec4(0.0, 0.0, 0.0, 1.0));
	
	vec3 rNormal = vec3(Model * vec4(normal, 0.0));
	float lit = clamp(dot(rNormal, light), 0.1, 1.0);
	color = vec4(lit * pColor, 1.0);// - 0.006 * sPos.z;//vec4(abs(pos) * pColor, 1.0);*/
}