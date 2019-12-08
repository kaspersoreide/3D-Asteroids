#version 330 core
layout(points) in;
layout(line_strip, max_vertices = 6) out;

in Data{
	vec3 theColor;
	vec2 uv;
} vin[];

out Data{
	vec3 theColor;
	vec2 uv;
} vout;

const vec2 pentagon[5] = {
	vec2(0.0, 1.0),
	vec2(-0.9510564735794738, 0.3090171258402609), 
	vec2(-0.5877854138262403, -0.8090168770137712), 
	vec2(0.5877850410559727, -0.8090171478472062), 
	vec2(0.9510566159650023, 0.3090166876225265)
};

void main() {
	vec4 p = gl_in[0].gl_Position;

	for (int i = 0; i < 6; i++) {
		vout.theColor = vin[0].theColor;
		vout.uv = vin[0].uv;
		gl_Position = vec4(p.xy + 0.01 * pentagon[i % 5], p.yw);
		EmitVertex();
	}

	EndPrimitive();
}