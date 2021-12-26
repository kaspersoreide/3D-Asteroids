#version 330 core

layout(location = 0) in vec3 inPos1;
layout(location = 1) in vec3 inVel1;
layout(location = 2) in vec3 inPos2;
layout(location = 3) in vec3 inVel2;

out vec3 outPos1;
out vec3 outVel1;
out vec3 outPos2;
out vec3 outVel2;

uniform vec3 origin;
uniform float radius;

// A single iteration of Bob Jenkins' One-At-A-Time hashing algorithm.
uint hash(uint x) {
	x += (x << 10u);
	x ^= (x >> 6u);
	x += (x << 3u);
	x ^= (x >> 11u);
	x += (x << 15u);
	return x;
}

// Compound versions of the hashing algorithm internet person whipped together.
uint hash(uvec2 v) { return hash(v.x ^ hash(v.y)); }
uint hash(uvec3 v) { return hash(v.x ^ hash(v.y) ^ hash(v.z)); }
uint hash(uvec4 v) { return hash(v.x ^ hash(v.y) ^ hash(v.z) ^ hash(v.w)); }

// Construct a float with half-open range [0:1] using low 23 bits.
// All zeroes yields 0.0, all ones yields the next smallest representable value below 1.0.
float floatConstruct(uint m) {
	const uint ieeeMantissa = 0x007FFFFFu; // binary32 mantissa bitmask
	const uint ieeeOne = 0x3F800000u; // 1.0 in IEEE binary32

	m &= ieeeMantissa;                     // Keep only mantissa bits (fractional part)
	m |= ieeeOne;                          // Add fractional part to 1.0

	float  f = uintBitsToFloat(m);       // Range [1:2]
	return f - 1.0;                        // Range [0:1]
}

// Pseudo-random value in half-open range [-1:1].
float random(float x) { return floatConstruct(hash(floatBitsToUint(x))); }
float random(vec2  v) { return floatConstruct(hash(floatBitsToUint(v))); }
float random(vec3  v) { return floatConstruct(hash(floatBitsToUint(v))); }
float random(vec4  v) { return floatConstruct(hash(floatBitsToUint(v))); }

void main() {
	if (length(inPos2) < 0.1) {
		vec3 pos = vec3(
			random(inVel2.x),
			random(inVel2.x + 0.1),
			random(inVel2.x + 0.2)
		) - 0.5;
		outVel2 = 0.4 * pos;
		pos = origin + random(pos) * radius * normalize(pos);
		outPos1 = outPos2 = pos;
		outVel1 = vec3(0.0);
	}
	else {
		/*outPos = inPos + 0.009 * vec3(
		inPos.y * inPos.z,
		-inPos.x * inPos.z,
		inPos.x * inPos.y
		);*/
		outPos1 = inPos2;
		/*outPos2 = inPos2 + 0.004 * vec3(
			10.0 * (inPos2.y - inPos2.x),
			inPos2.x * (28.0 - inPos2.z) - inPos2.y,
			inPos2.x * inPos2.y - 8.0 * inPos2.z / 3.0
		);*/
		outPos2 = inPos2 + inVel2;
		//outPos2 = inPos2 + inColor2 + 0.001 * vec3(inPos2.y, -inPos2.x, 0.0);
		vec3 d = inPos2 - origin;
		outVel2 = inVel2 + d * 0.1 / length(d);// + 0.00001 * cross(inPos2, vec3(0.0, 1.0, 0.0));
		outVel1 = inVel1;
	}
}