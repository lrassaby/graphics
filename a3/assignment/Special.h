#ifndef SPECIAL_H
#define SPECIAL_H

#include "Round.h"

class Special : public Round {
public:
    Special();
    ~Special();

    void normalByPoint(double x, double y, double z);
    void calcTriangles();
    double radius(double y);
};
#endif
