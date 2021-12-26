#version 330 core

in vec2 screenPos;
out vec4 FragColor;

uniform mat4 MVP[64];
uniform vec3 color[64];
uniform float size[64];
uniform float t[64];
uniform int count;

void main() {
	FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	for (int i = 0; i < count; i++) {
		vec3 p = vec3(MVP[i] * vec4(0.0, 0.0, 0.0, 1.0));
		if (p.z > 0.0) {
			vec2 d = vec2(16.0 / 9, 1.0) * (p.xy / p.z - screenPos);
			FragColor += vec4(clamp(
				1.0 - (5.9 * length(d) + 0.05 * p.z) / size[i] - 0.05 * t[i],
				0.0, 0.7) * color[i], 
				0.0
			);
		}
	}
	//FragColor = vec4(1.0) / length(screenPos);
}