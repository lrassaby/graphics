#include "Round.h"

Round::Round() { }; 
Round::~Round(){ };


double Round::radius(double y) {};

void Round::tessellateCircle(Point circle[]) {
  Point center(0, circle[0].at(1), 0);
  for (int i = 0; i < m_segmentsX; i++) {
    if (center.at(1) == 0.5) addTriangle(center, circle[(i+1) % m_segmentsX], circle[i]);
    else addTriangle(center, circle[i], circle[(i+1) % m_segmentsX]);
  }
}

void Round::calcAllCirclePoints(double y, double r, Point circle[]) { 
  double theta, x, z;
  for(int i = 1; i <= m_segmentsX; i++) {   
    theta = (2 * M_PI * i) / (double) m_segmentsX;
    x = r * cos(theta);
    z = r * sin(theta);
    circle[i-1] = Point(x, y, z);
  }
}

void Round::tessellateRadius() {
  double y_base = -.5;
  Point *lower = new Point[m_segmentsX];
  Point *upper = new Point[m_segmentsX];
  for(int i = 0; i < m_segmentsY; i++) {
    double y1 = (double) i / (double) m_segmentsY + y_base;
    double y2 = (double) (i + 1) / (double) m_segmentsY + y_base;
    calcAllCirclePoints(y1, radius(y1), lower);
    calcAllCirclePoints(y2, radius(y2), upper);
    for (int j = 0; j <= m_segmentsX; j++) {
      addTriangle(lower[j       % m_segmentsX], 
                  upper[(j + 1) % m_segmentsX],
                  lower[(j + 1) % m_segmentsX]); 
      addTriangle(lower[j       % m_segmentsX], 
                  upper[j       % m_segmentsX],
                  upper[(j + 1) % m_segmentsX]); 
    }
  }
  delete [] lower;
  delete [] upper;
}
