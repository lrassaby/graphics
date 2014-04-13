#include <Math.h>
#include "Cone.h"

Cone::Cone() {
  cross_product_normals = false;
  point_normals = true;
}

Cone::~Cone() {}

void Cone::calcTriangles() {
  /* bottom circle */
  Point *circle = new Point[m_segmentsX];

  calcAllCirclePoints(-.5, .5, circle);
  tessellateCircle(circle);

  tessellateRadius();
  delete [] circle;
}

double Cone::radius(double y) {
  return .25 - .5 * y;
}

void Cone::normalByPoint(double x, double y, double z){
#define N(a,b,c) normalizeNormal(a, b, c);
  if(y == 0.5) {
    N(0, 1, 0);
    return;
  } else if (y == -0.5 && x == 0 && z == 0) {
    N(0, -1, 0);
  } else if (y == -0.5) {
    //return;
  }
  double theta = atan(z/x);
  double length = 2/sqrt(5) + sqrt(x * x + z * z);
  double x_normal = length * cos(theta);
  double z_normal = length * sin(theta);
  if (x < 0) {
    x_normal = -1 * x_normal;
    z_normal = -1 * z_normal;
  }
  Point a(x, y, z), b(x_normal, y + 1/sqrt(5), z_normal);
  normalizeNormal(b-a);

#undef N
};
