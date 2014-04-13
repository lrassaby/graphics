#ifndef ROUND_H
#define ROUND_H

#include "Shape.h"

class Round : public Shape {
  public:
    Round(); 
    ~Round();

  protected:
    virtual double radius(double y);
    void tessellateCircle(Point circle[]);
    void calcAllCirclePoints(double y, double r, Point circle[]);
    void tessellateRadius();
};
#endif
