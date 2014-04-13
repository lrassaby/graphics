#ifndef CUBE_H
#define CUBE_H

#include "Face.h"

class Cube : public Face {
public:
	Cube();
	~Cube();
    void normalByPoint(double x, double y, double z);
    void calcTriangles();
};

#endif
