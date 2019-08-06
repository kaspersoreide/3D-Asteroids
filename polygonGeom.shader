#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 6) out;

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
	if (alive) {
		vout.vNormal = vin[0].vNormal;
		vout.center = vin[0].center;
		for (int i = 0; i < 3; i++) {
			vout.rPos = vin[i].rPos;
			gl_Position = gl_in[i].gl_Position;
			EmitVertex();
		}
		EndPrimitive();
	}
	else {
		vec3 actualSpin = spin;//vec3(Model * vec4(spin, 0.0));
		vout.vNormal = vin[0].vNormal;
		vout.center = vin[0].center;
		vec4 transformedNormal = VP * vec4(t * (3.0 * vin[0].vNormal + cross(vin[0].vNormal, 100.0 * actualSpin)), 0.0);
		vec3 extraVertex = vin[0].center;
		for (int i = 0; i < 3; i++) {
			vout.rPos = vin[i].rPos + transformedNormal.xyz;
			gl_Position = gl_in[i].gl_Position + transformedNormal;
			EmitVertex();
		}
		vout.rPos = extraVertex + transformedNormal.xyz;
		vout.vNormal = vin[0].vNormal;
		gl_Position = transformedNormal + VP * vec4(extraVertex, 1.0);
		EmitVertex();
		vout.vNormal = vin[0].vNormal;
		for (int i = 0; i < 2; i++) {
			vout.rPos = vin[i].rPos + transformedNormal.xyz;
			gl_Position = gl_in[i].gl_Position + transformedNormal;
			EmitVertex();
		}
		EndPrimitive();
	}
}