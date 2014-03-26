#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shape.h"

class Cylinder : public Shape {
public:
	Cylinder() {};
	~Cylinder() {};

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
