#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in Data{
	vec3 vNormal;
	vec3 rPos;
	vec3 center;
} vin[];

out Data{
	vec3 vNormal;
	vec3 rPos;
	vec3 center;
} vout;

uniform bool alive;
uniform float t;
uniform mat4 VP;
uniform mat4 Model;
uniform vec3 spin;

void main() {
	vout.vNormal = vin[0].vNormal;
	vout.center = vin[0].center;
	for (int i = 0; i < 3; i++) {
		vout.rPos = vin[i].rPos;
		gl_Position = gl_in[i].gl_Position + vec4(vin[i].vNormal * t, 0.0);
		EmitVertex();
	}
	EndPrimitive();
}