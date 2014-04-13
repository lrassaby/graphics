#ifndef SHAPE_H
#define SHAPE_H

#include <GL/glui.h>
#include <vector>
#include <map>
#include <set>
#include "Algebra.h"

class Shape {
public:
    Shape();
    ~Shape();

	void setSegments(int x, int y);

    virtual void draw();
	virtual void drawNormal();
    virtual void calcTriangles();

protected:

    struct Triangle {
      Point a, b, c;
    };

	void normalizeNormal (float x, float y, float z);
	void normalizeNormal (Vector v);

	int m_segmentsX, m_segmentsY;
    bool redraw, cross_product_normals, point_normals;
    std::vector<Triangle> triangles;

    void clearTriangles();
    virtual void normalByPoint(double x, double y, double z);

    void addTriangle(Point a, Point b, Point c); 

    void drawAllTriangles(); 
};

#endif
