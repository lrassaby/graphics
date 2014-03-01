#ifndef CONE_H
#define CONE_H

#include "Round.h"

class Cone : public Round{
public:
	Cone();
	~Cone();

    void normalByPoint(double x, double y, double z);
    double radius(double y);
    void calcTriangles();
};

#endif
