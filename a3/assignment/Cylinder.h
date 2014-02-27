#ifndef CYLINDER_H
#define CYLINDER_H

#include "Round.h"

class Cylinder : public Round {
public:
	Cylinder(); 
    ~Cylinder();

    void normalByPoint(double x, double y, double z);
    void calcTriangles();
    double radius(double y);
};
#endif
