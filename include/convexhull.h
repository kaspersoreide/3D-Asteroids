#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#include <vector>

using namespace std;
using namespace glm;

struct Edge {
	vec3 p0, p1, n;
	bool visited;
	Edge(vec3 _p0, vec3 _p1, vec3 _n) {
		p0 = _p0;
		p1 = _p1;
		n = _n;
		visited = false;
	}
	inline bool operator==(const Edge& e1) const {
		if (e1.p0 == p0 && e1.p1 == p1) return true;
		if (e1.p0 == p1 && e1.p1 == p0) return true;
		return false;
	}
};

struct Triangle {
	vec3 pts[3], normal;
	Triangle(vec3 p0, vec3 p1, vec3 p2, vec3 _normal) {
		normal = _normal;
		pts[0] = p0;
		pts[1] = p1;
		pts[2] = p2;
	}
};

Edge findInitialEdge(const vector<vec3>& pts);

void addEdge(vector<Edge>& edges, const Edge& edge);

vec3 findPoint(const vector<vec3>& pts, const Edge& edge);

vector<Triangle> giftWrap(const vector<vec3>& points);