#ifndef FACE_H
#define FACE_H

#include "Shape.h"
class Face : public Shape {
  public:
    Face(); 
    ~Face();

  protected:
    void tessellateFace(Point **face);
    void calcAllFacePoints(double x_init, double x_end, 
                           double y_init, double y_end,
                           double z_init, double z_end, Point **face); 
};

#endif
