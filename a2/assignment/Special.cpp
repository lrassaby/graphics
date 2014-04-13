#include "Special.h"
#include <math.h>

Special::Special() {
  cross_product_normals = true;
  point_normals = false;
};
Special::~Special() {};

void Special::calcTriangles() {
  Point *circle = new Point[m_segmentsX];

  tessellateRadius();
  calcAllCirclePoints(-.5, .5, circle);
  tessellateCircle(circle);
  calcAllCirclePoints(-.47, .5, circle);
  tessellateCircle(circle);
  calcAllCirclePoints(.35, .3, circle);
  tessellateCircle(circle);

  delete [] circle;
}

double Special::radius(double y) {
  if (y > -.4) {
    if (y > .35) return 0;
    return .3;
  } else {
    return .5;
  }
}

void Special::normalByPoint(double x, double y, double z){
#define N(a,b,c) normalizeNormal(a, b, c);
  N(x, y, z);
#undef N
}
