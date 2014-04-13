#include "Face.h"

Face::Face() { }; 
Face::~Face(){ };

void Face::tessellateFace(Point **face) {
  for(int i = 0; i < m_segmentsX; i++) {
    for(int j = 0; j < m_segmentsY; j++) {
      addTriangle(face[i][j], face[i + 1][j], face[i+1][j+1]);
      addTriangle(face[i][j], face[i+1][j+1], face[i][j+1]);
    }
  }
}

void Face::calcAllFacePoints(double x_init, double x_end, 
    double y_init, double y_end,
    double z_init, double z_end, Point **face) {  
  double dx  = (x_end - x_init) / m_segmentsX;
  double dy  = (y_end - y_init) / m_segmentsY;
  double dz1 = (z_end - z_init) / m_segmentsX; 
  double dz2 = (z_end - z_init) / m_segmentsY; 

  for(int i = 0; i <= m_segmentsX; i++) {
    for(int j = 0; j <= m_segmentsY; j++) {
      if(dy != 0) {
        face[i][j] = 
          Point(x_init + i * dx, y_init + j * dy, z_init + i * dz1); 
      } else {
        face[i][j] = Point(x_init + i * dx, y_init, z_init + j * dz2); 
      }
    }
  }
}


