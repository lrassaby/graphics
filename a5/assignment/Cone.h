#ifndef COHE_H
#define CONE_H

#include "Shape.h"

class Cone : public Shape {

public:
	Cone() { num_points = x_points * y_segments; };

	~Cone() {};

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
        double radius_increment = SCALE/y_segments;
        num_points = x_points * y_segments;
        points = new Point[num_points];

        for (int i = 0; i < y_segments; i++) {
            for (int j = 0; j < x_points; j++) {
                points[j + (x_points * i)] = 
                    Point((radius_increment * (i + 1)) * sin(j * theta), 
                          SCALE - ((i + 1) * y_increment),
                          (radius_increment * -1 * (i + 1)) * cos(j * theta));
            }
        }
    };

    /* 
     * draw_top_normals - draws normals derived from the sides at the 
     * top tip of the cone.
     * */
    void draw_top_normals()
    {
        double theta = 2*PI/x_segments;
        double slope = SCALE/HEIGHT;

        for (int i = num_points - x_points; i < num_points - 1; i++) {
            glBegin(GL_LINES);
                glVertex3f(top[X], top[Y], top[Z]);
                glVertex3f(top[X] + (N_MAG * cos((i % x_points) * theta)), 
                           top[Y] + (N_MAG * slope), 
                           top[Z] + (N_MAG * -sin((i % x_points) * theta)));
            glEnd(); 
        }
    };
    
    /*
     * draw_side_normals - draw the cone normals, minus the tip and the
     * bottom face.
     */
	void draw_side_normals() 
    {
        double theta = 2*PI/x_segments;
        double slope = SCALE/HEIGHT;
        
        for (int i = 0; i < num_points; i++) {
            glBegin(GL_LINES);
                glColor3f(1.0, 0.0, 0.0);
                glVertex3f(points[i][X], 
                           points[i][Y], 
                           points[i][Z]); 
                glVertex3f(points[i][X] + 
                                (N_MAG * sin((i % x_points) * theta)), 
                           points[i][Y] + (N_MAG * slope), 
                           points[i][Z] + 
                                (N_MAG * -cos((i % x_points) * theta)));
            glEnd();
        }
	};

    /** override function implmented in Shape superclass **/
    void draw_top_center_normals() {};
};

#endif
