#ifndef SPHERE_H 
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape {

public:
	Sphere() { num_points = x_points * (y_segments - 1); };

	~Sphere() {};

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
        double phi = PI/y_segments; /* increment in phi */
        num_points = x_points * y_points;
        points = new Point[num_points];

        for (int i = 0; i < y_segments; i++) {
            for (int j = 0; j < x_points; j++) {
                points[j + (x_points * i)] = 
                    Point(SCALE * cos(j * theta) * sin((i + 1) * phi),
                          SCALE * cos((i + 1) * phi),
                          SCALE * sin(j * theta) * sin((i + 1) * phi));
            }
        }
    };

    /* draw_side_normals - draw the sphere normals */
	void draw_side_normals() 
    {
        for (int i = 0; i < num_points; i++) {
            glBegin(GL_LINES);
                glVertex3f(points[i][X], 
                           points[i][Y], 
                           points[i][Z]);
                glVertex3f(points[i][X] * 1.2,
                           points[i][Y] * 1.2,
                           points[i][Z] * 1.2);
            glEnd();
        }
	};

    /** override functions implmented in Shape superclass **/
    void draw_top_normals() {};
    void draw_bottom_normals() {};
};

#endif
