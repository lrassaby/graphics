#ifndef SPHERE_H
#define SPHERE_H

#include "Round.h"

class Sphere : public Round {
public:
	Sphere();
	~Sphere();

    void normalByPoint(double x, double y, double z);
    void calcTriangles();
    double radius(double y);
};
#endif
