#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape {
public:
	Sphere() {};
	~Sphere() {};

	void draw() {
	};

	void drawNormal() {
	};

	double Intersect(Point eyePointP, Vector rayV, Matrix transformMatrix) {
		return 0;
	};

	Vector findIsectNormal(Point eyePoint, Vector ray, double dist) {
		Vector v;
		return v;
	};
};

#endif
