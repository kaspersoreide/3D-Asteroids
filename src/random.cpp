#include "random.h"

unsigned int RNG::seed;

// A single iteration of Bob Jenkins' One-At-A-Time hashing algorithm.
unsigned int RNG::hash(unsigned int x) {
	x += (x << 10u);
	x ^= (x >> 6u);
	x += (x << 3u);
	x ^= (x >> 11u);
	x += (x << 15u);
	return x;
}

unsigned int RNG::hash(unsigned int* x) { 
	*x = hash(*x);
	return *x;
} 

unsigned int RNG::randomUint() {
	return(hash(&seed));
}

// Compound versions of the hashing algorithm internet person whipped together.
unsigned int RNG::hash(uvec2 v) { return hash(v.x ^ hash(v.y)); }
unsigned int RNG::hash(uvec3 v) { return hash(v.x ^ hash(v.y) ^ hash(v.z)); }
unsigned int RNG::hash(uvec4 v) { return hash(v.x ^ hash(v.y) ^ hash(v.z) ^ hash(v.w)); }

// Construct a float with half-open range [0:1] using low 23 bits.
// All zeroes yields 0.0, all ones yields the next smallest representable value below 1.0.
float RNG::floatConstruct(unsigned int m) {
	const unsigned int ieeeMantissa = 0x007FFFFFu; // binary32 mantissa bitmask
	const unsigned int ieeeOne = 0x3F800000u; // 1.0 in IEEE binary32

	m &= ieeeMantissa;                     // Keep only mantissa bits (fractional part)
	m |= ieeeOne;                          // Add fractional part to 1.0

		//int to float 
	float  f = *(float*)(&m);       // Range [1:2]
	return f - 1.0;                        // Range [0:1]
}

float RNG::randomFloat() {
	return floatConstruct(hash(&seed));
}

// Pseudo-random value in half-open range [0:1].
float RNG::random(float x) { return floatConstruct(hash(*(unsigned int*)(&x))); }

float RNG::noise(vec2 p) {
	const float K1 = 0.366025404f; // (sqrt(3)-1)/2;
	const float K2 = 0.211324865f; // (3-sqrt(3))/6;

	vec2 i = vec2(
		std::floor(p.x + (p.x + p.y)*K1),
		std::floor(p.y + (p.x + p.y)*K1)
	);

	vec2 a = vec2(
		p.x - i.x + (i.x + i.y)*K2,
		p.y - i.y + (i.x + i.y)*K2
	);
	vec2 o = vec2(0.0f);//step(a.yx, a.xy);
	if (a.y > a.x) {
		o.x = 0.0f;
		o.y = 1.0f;
	}
	else {
		o.x = 1.0f;
		o.y = 0.0f;
	}
	vec2 b = a - o + vec2(K2);
	vec2 c = a - vec2(1.0f) + vec2(2.0f*K2);

	vec3 h(
		std::max(0.5f - dot(a, a), 0.0f),
		std::max(0.5f - dot(b, b), 0.0f),
		std::max(0.5f - dot(c, c), 0.0f)
	);
	//vec3 h = max(vec3(0.5) - vec3(dot(a, a), dot(b, b), dot(c, c)), 0.0);

	vec3 n = vec3(
		h.x*h.x*h.x*h.x*dot(a, vec2(random(i.x + 0.0), random(i.y + 0.0))),
		h.y*h.y*h.y*h.y*dot(b, vec2(random(i.x + o.x), random(i.y + o.y))),
		h.z*h.z*h.z*h.z*dot(c, vec2(random(i.x + 1.0), random(i.y + 1.0)))
	);

	return dot(n, vec3(70.0f));
}

vec3 RNG::randomvec3() {
	return vec3(
		randomFloat(),
		randomFloat(),
		randomFloat()
	);
}