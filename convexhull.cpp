#include "convexhull.h"

GLuint incrementalHull(vector<vec3> points) {
	//we assume the center of the hull is the origin
	struct Vertex {
		vec3 p;
		vec3 n;
	};
	struct Face {
		Vertex v[3];
	};
	vector<vec3> hull;
	vector<Face> faces;
	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);
	for (int i = 0; i < 4; i++) {
		auto it = points.begin() + RNG::randomUint() % points.size();
		//initial hull (tetrahedron)
		hull.push_back(*it);
		points.erase(it);
	}
	for (int i = 0; i < 4; i++) {
		Face f;
		vec3 p[3] = {
			hull[i % 3],
			hull[(i + 1) % 3],
			hull[(i + 2) % 3]
		};
		vec3 n = normalize(cross(p[1] - p[0], p[2] - p[0]));
		if (dot(n, -p[0]) > 0.0) {
			//we need to make faces counterclockwise
			n *= -1;
			vec3 tmp = hull[1];
			hull[1] = hull[2];
			hull[2] = tmp;
		}
		for (int j = 0; j < 3; j++) {
			Vertex vert;
			vert.p = p[0];
			vert.n = n;
			f.v[j] = vert;
			faces.push_back(f);
		}
	}
	while (!points.empty()) {
		vec3 p = points.back();
		bool isOutside = true;
		auto baseFace = faces.begin();
		float maxAlignment = 0.0f;
		for (auto it = faces.begin(); it < faces.end(); ++it) {
			//face with lowest dot product is base of extrusion
			Face f = *it;
			vec3 d = p - f.v[0].p;
			float dp = dot(d, f.v[0].n);
			if (dp < 0.0f) {
				isOutside = false;
				break;
			}
			if (dp > maxAlignment) {
				maxAlignment = dp;
				baseFace = it;
			}
		}
		if (isOutside) {
			//extrusion part: replace f with three new faces
			hull.push_back(p);
			Face f = *baseFace;
			for (int i = 0; i < 3; i++) {
				//3 new faces
				Face nf;
				vec3 v0 = f.v[i].p;
				vec3 v1 = f.v[(i + 1) % 3].p;
				vec3 n = normalize(cross(v1 - v0, p - v0));
				nf.v[0].p = v0;
				nf.v[0].n = n;
				nf.v[1].p = v1;
				nf.v[1].n = n;
				nf.v[2].p = p;
				nf.v[2].n = n;
			}
		}
		points.pop_back();
	}
}