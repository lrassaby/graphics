#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shape.h"

class Cylinder : public Shape {

public:
	Cylinder() { num_points = x_points * y_points; };

	~Cylinder() {};
   
private:
    /* 
     * calculate_points - (re)calculate point locations for all faces at
     * program initiation and upon change in either m_segmentsX or m_segmentsY
     */
    void calculate_points()
    {
        if (points != NULL) {
            delete [] points;
        }
        double theta = 2*PI/x_segments;
        double y_increment = 1.0/y_segments;
        num_points = x_points * y_points;
        points = new Point[num_points];
        
        for (int i = 0; i < y_points; i++) {
            for (int j = 0; j < x_points; j++) {
                points[j + (x_points * i)] = 
                    Point(SCALE * sin(j * theta), 
                          SCALE - (i * y_increment),
                          (-1.0 * SCALE) * cos(j * theta));
            }
        }
    };

    /*
     * draw_side_normals - draw the cylinder normals, minus the top and
     * bottom face.
     */
	void draw_side_normals() 
    {
        double theta = 2*PI/x_segments;
        
        for (int i = 0; i < num_points - 1; i++) {
            glBegin(GL_LINES);
                glVertex3f(points[i][X], 
                           points[i][Y], 
                           points[i][Z]); 
                glVertex3f(points[i][X] + N_MAG * sin((i % x_points) * theta),
                           points[i][Y], 
                           points[i][Z] 
                            + (-1.0 * N_MAG) * cos((i % x_points) * theta));
            glEnd();
        }
	};
};

#endif
