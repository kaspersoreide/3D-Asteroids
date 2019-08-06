#version 330 core

in Data{
	vec3 vNormal;
	vec3 rPos;
	vec3 center;
};
out vec4 FragColor;

uniform bool alive;
uniform float t;
uniform vec3 pColor;
uniform vec3 cPos;
uniform mat4 VP;

void main() {
	//vec2 d = vec2(VP * vec4(rPos - center, 1.0));//vec2(VP * vec4(rPos, 0.0) - VP * vec4(center, 0.0));
	//float dist2 = dot(d, d);
	vec3 lightDir = normalize(rPos - center);
	vec3 viewDir = normalize(rPos - cPos);
	vec3 reflectDir = reflect(lightDir, vNormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
	vec3 specular = spec * pColor;
	if (alive) {
		FragColor = vec4(specular, 1.0);
	}
	else {
		FragColor = (1.0 - 0.05 * t) * vec4(specular, 1.0);
	}
	
	//FragColor = vec4(1.0);
}