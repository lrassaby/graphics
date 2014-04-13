#include "Cube.h" 
Cube::Cube() {
  cross_product_normals = true;
  point_normals = false;
}

Cube::~Cube() {}


void Cube::calcTriangles() {
#define l 0.5
#define facePts(a, b, c, d, e, f) calcAllFacePoints(a l, b l, c l, d l, e l, f l, face)
 Point **face = (Point **) malloc(sizeof(Point *) * (m_segmentsX + 1));
 for(int i = 0; i <= m_segmentsX; i++) {
   face[i] = (Point *) malloc(sizeof(Point) * (m_segmentsY+1));
 }
 /* x init, x end, y init, y end, z init, z end */
 /* front face */
 facePts(-, +, -, +, +, +); 
 tessellateFace(face); 
 /* back face */
 facePts(+, -, -, +, -, -); 
 tessellateFace(face);
 /* left face */
 facePts(-, -, -, +, -, +);
 tessellateFace(face); 
 /* right face */
 facePts(+, +, -, +, +, -);
 tessellateFace(face); 
 /* top face */
 facePts(-, +, +, +, +, -); 
 tessellateFace(face); 
 /* bottom face */
 facePts(-, +, -, -, -, +);
 tessellateFace(face); 
#undef l
#undef facePts
}

void Cube::normalByPoint(double x, double y, double z) {
#define N(a,b,c) normalizeNormal(a, b, c);
  if (x == 0.5) {
    N(1, 0, 0);
  } else if (x == -0.5) {
    N(-1, 0, 0);
  }
  if (y == 0.5) {
    N(0, 1, 0);
  } else if (y == -0.5) {
    N(0, -1, 0);
  }
  if (z == 0.5) {
    N(0, 0, 1);
  } else if (z == -0.5) {
    N(0, 0, -1);
  }
#undef N
}
