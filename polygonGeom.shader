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
		vout.vNormal = vin[0].vNormal;
		vout.center = vin[0].center;
		vec4 transformedNormal = VP * vec4(t * vin[0].vNormal, 0.0);
		vec3 extraVertex = 0.33333 * (vin[0].rPos + vin[1].rPos + vin[2].rPos)
			- 5.5f * vin[0].vNormal;
		for (int i = 0; i < 3; i++) {
			vout.rPos = vin[i].rPos;
			gl_Position = gl_in[i].gl_Position + transformedNormal;
			EmitVertex();
		}
		vout.rPos = extraVertex;
		vout.vNormal = vin[0].vNormal;
		gl_Position = transformedNormal + VP * vec4(extraVertex, 1.0);
		EmitVertex();
		vout.vNormal = vin[0].vNormal;
		for (int i = 0; i < 2; i++) {
			vout.rPos = vin[i].rPos;
			gl_Position = gl_in[i].gl_Position + transformedNormal;
			EmitVertex();
		}
		EndPrimitive();
	}
	//vec3 p[3] = {
	//	gl_in[0].gl_Position.xyz,
	//	gl_in[1].gl_Position.xyz,
	//	gl_in[2].gl_Position.xyz
	//};
	//
	//gl_Position = gl_in[0].gl_Position;
	//EmitVertex();
	//
	//gl_Position = gl_in[1].gl_Position;
	//EmitVertex();
	//
	//gl_Position = gl_in[2].gl_Position;
	//EmitVertex();
	//
	//gl_Position = vec4(0.33333f * (p[0] + p[1] + p[2]) + cross(p[1] - p[0], p[2] - p[0]), 1.0);
	//EmitVertex();
	//
	//gl_Position = gl_in[0].gl_Position;
	//EmitVertex();
	//
	//gl_Position = gl_in[1].gl_Position;
	//EmitVertex();
	//
}