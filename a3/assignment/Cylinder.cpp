#include "Cylinder.h"
#include <math.h>

Cylinder::Cylinder() {
  cross_product_normals = true;
  point_normals = false;
}

Cylinder::~Cylinder() {}

double Cylinder::radius(double y) {
  return 0.5;
}


void Cylinder::calcTriangles() {
  Point *circle = new Point[m_segmentsX];
  calcAllCirclePoints(.5, .5, circle);
  tessellateCircle(circle);
  calcAllCirclePoints(-.5, .5, circle);
  tessellateCircle(circle);
  tessellateRadius();
  delete [] circle;
}


void Cylinder::normalByPoint(double x, double y, double z){
#define N(a,b,c) normalizeNormal(a, b, c);
  if(y == .5 || y == -.5)
    N(0, y, 0)
  else 
    N(x, 0, z)
#undef N
};
