#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"

class Cube : public Shape {
public:
	Cube() {};
	~Cube() {};

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
