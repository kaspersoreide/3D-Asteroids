#include "convexhull.h"

Edge findInitialEdge(const vector<vec3>& pts) {
	vec3 p0, p1;
	p0 = p1 = vec3(-FLT_MAX);
	for (vec3 p : pts) {
		if (p.x > p0.x) {
			p1 = p0;
			p0 = p;
			continue;
		}
		if (p.x > p1.x) {
			p1 = p;
		}
	}
	vec3 l = p1 - p0;
	vec3 n = normalize(p0 - l * dot(p0, l) / dot(l, l));
	return Edge(p0, p1, n);
}

void addEdge(vector<Edge>& edges, const Edge& edge) {
	//cout << "Considering adding edge: ";
	//printVec3(edge.p0);
	//printVec3(edge.p1);
	bool existing = false;
	for (int k = 0; k < edges.size(); k++) {
		if (edge == edges[k]) {
			edges[k].visited = true;
			existing = true;
			break;
		}
	}
	if (existing) {
		//cout << "...it was already there\n";
		return;
	}
	edges.push_back(edge);
	//cout << "...added\n";
}

vec3 findPoint(const vector<vec3>& pts, const Edge& edge) {
	//float maxdp = -FLT_MAX;
	vec3 np = vec3(0.0f);
	//vec3 dir = cross(edge.n, edge.p1 - edge.p0);
	for (vec3 p : pts) {
		if (p == edge.p0) continue;
		if (p == edge.p1) continue;
		bool inside = false;
		//float dp = determinant(mat3(normalize(p - edge.p0), edge.n, normalize(edge.p1 - edge.p0)));
		vec3 normal = cross(edge.p1 - edge.p0, p - edge.p0);
		for (vec3 p2 : pts) {
			if (p2 == p) continue;
			if (p2 == edge.p0) continue;
			if (p2 == edge.p1) continue;
			if (dot(p2 - p, normal) > 0.0f) {
				inside = true;
				break;
			}
		}
		if (!inside) {
			np = p;
			break;
		}
	}
	return np;
}


vector<Triangle> giftWrap(const vector<vec3>& pts) {
	vector<Triangle> hull;
	vector<Edge> edges;
	edges.push_back(findInitialEdge(pts));
	//loop for adding vertices to hull
	for (int i = 0; i < edges.size(); i++) {
		Edge e = edges[i];
		//cout << "Processing edge nr " << i << ":\n";
		//printVec3(e.p0);
		//printVec3(e.p1);
		if (e.visited) {
			//cout << "e was already visited, ignoring\n";
			continue;
		}
		vec3 np = findPoint(pts, e);
		vec3 n = normalize(cross(e.p1 - e.p0, np - e.p0));
		hull.push_back(Triangle(e.p0, e.p1, np, n));
		//now we check if edges of new triangle is in edges
		//if we dont do this it will go on forever
		addEdge(edges, Edge(e.p0, np, n));
		addEdge(edges, Edge(np, e.p1, n));
	}
	return hull;
}