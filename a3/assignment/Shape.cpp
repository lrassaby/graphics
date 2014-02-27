#include "Shape.h"

Point startpt;
bool line_normals_only;

Shape::Shape() {
  redraw = true;
  line_normals_only = false;
  cross_product_normals = false;
  point_normals = false;
}
Shape::~Shape() {};

void Shape::setSegments(int x, int y) {
  redraw = (x != m_segmentsX || y != m_segmentsY);
  m_segmentsX = x;
  m_segmentsY = y;
}

void Shape::draw() {
  if (redraw) {
    clearTriangles();
    calcTriangles();
  }
  drawAllTriangles();
}

void Shape::drawNormal() {
  line_normals_only = true; 
  if (redraw) {
    clearTriangles();
    calcTriangles();
  }
  for (int i = 0; i < triangles.size(); i++) {
    Triangle tri = triangles[i];
    if (cross_product_normals) {
      startpt = (tri.a + tri.b + tri.c) / 3;
      normalizeNormal(cross(tri.b-tri.a, tri.c-tri.b));
    } else {
      startpt = tri.a;
      normalByPoint(startpt.at(0), startpt.at(1), startpt.at(2));
      startpt = tri.b;
      normalByPoint(startpt.at(0), startpt.at(1), startpt.at(2));
      startpt = tri.c;
      normalByPoint(startpt.at(0), startpt.at(1), startpt.at(2));
    }
  }
  line_normals_only = false;
}

void Shape::calcTriangles() {};

void Shape::normalizeNormal (float x, float y, float z) {
  normalizeNormal (Vector(x, y, z));
}

void Shape::normalizeNormal (Vector v) {
  #define vlen .1
  v.normalize();
  if (line_normals_only) {
    glBegin(GL_LINES); 
      glVertex3f(startpt.at(0), startpt.at(1), startpt.at(2));
      Point endpt = startpt + (v * vlen); 
      glVertex3f(endpt.at(0), endpt.at(1), endpt.at(2));
    glEnd();
  }
  else glNormal3dv(v.unpack());
  #undef vlen
}

void Shape::clearTriangles() {
  triangles.clear();
}

void Shape::normalByPoint(double x, double y, double z){ };

void Shape::addTriangle(Point a, Point b, Point c) {
  Triangle tri;
  tri.a = a;
  tri.b = b;
  tri.c = c;
  triangles.push_back(tri);
}

void Shape::drawAllTriangles() {
  for (int i = 0; i < triangles.size(); i++) {
    Triangle tri = triangles[i];
    Point a = tri.a, b = tri.b, c = tri.c;
    glBegin(GL_TRIANGLES);
    if (cross_product_normals) normalizeNormal(cross(b-a, c-b));
    if (point_normals) normalByPoint(a.at(0), a.at(1), a.at(2));
    glVertex3f(a.at(0), a.at(1), a.at(2));
    if (point_normals) normalByPoint(b.at(0), b.at(1), b.at(2));
    glVertex3f(b.at(0), b.at(1), b.at(2));
    if (point_normals) normalByPoint(c.at(0), c.at(1), c.at(2));
    glVertex3f(c.at(0), c.at(1), c.at(2));
    glEnd();
  }
}
