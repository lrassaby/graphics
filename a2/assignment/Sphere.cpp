#include "Sphere.h"

#include <math.h>

Sphere::Sphere() {
  cross_product_normals = false;
  point_normals = true;
}
Sphere::~Sphere() {}

void Sphere::calcTriangles() {
  tessellateRadius();
}

double Sphere::radius(double y) {
  return sqrt(.25 - y * y);
}

void Sphere::normalByPoint(double x, double y, double z){
#define N(a,b,c) normalizeNormal(a, b, c);
  N(x, y, z);
#undef N
};
