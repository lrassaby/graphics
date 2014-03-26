#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"

#define SIZE_X 1.0
#define SIZE_Y 1.0
#define SIDES 6

class Cube : public Shape {

public:
	Cube() 
    {
        int points_per_face = x_points * (y_points + 1);
        num_points = SIDES * points_per_face;
    };

	~Cube() {};

private:
    /* 
     * calculate_points - (re)calculate point locations for all faces at
     * program initiation and upon change in either m_segmentsX or m_segmentsY
     */
    void calculate_points()
    {
        int points_per_face = x_points * (y_points + 1);
        num_points = SIDES * points_per_face;

        double x_increment = SIZE_X/x_segments;
        double y_increment = SIZE_Y/y_segments;

        if (points != NULL) {
                delete [] points;
        }
        points = new Point[num_points];

        for (int i = 0; i < points_per_face - x_points; i++) {
            /* top */
            points[i] = 
                Point((-1.0 * SCALE) + ((i % x_points) * x_increment),
                      SCALE,
                      SCALE - ((i / x_points) * y_increment));
            /* bottom */
            points[i + points_per_face] = 
                Point(SCALE - ((i / x_points) * y_increment),
                      (-1.0 * SCALE),
                      (-1.0 * SCALE) + ((i % x_points) * x_increment));
            /* right */ 
            points[i + (2 * points_per_face)] = 
                Point(SCALE,
                      SCALE - ((i / x_points) * y_increment),
                      (-1.0 * SCALE) + ((i % x_points) * x_increment));
            /* left */
            points[i + (3 * points_per_face)] = 
                Point((-1.0 * SCALE),
                      (-1.0 * SCALE) + ((i % x_points) * x_increment),
                      SCALE - ((i / x_points) * y_increment));
            /* front */
            points[i + (4 * points_per_face)] = 
                Point((-1.0 * SCALE) + ((i % x_points) * x_increment),
                      (-1.0 * SCALE) + ((i / x_points) * y_increment),
                      SCALE);
            /* back */
            points[i + (5 * points_per_face)] = 
                Point(SCALE - ((i % x_points) * x_increment), 
                      (-1.0 * SCALE) + ((i / x_points) * y_increment),
                      (-1.0 * SCALE));
        }
    };

    /* 
     * one_side_normals - draws the given normal vectors for a single cube 
     * face, indicated by the given initial index into the points array.
     */
    void one_side_normals(int index, float x, float y, float z)
    {
        int points_per_face = x_points * (y_points + 1);

        for (int i = 0; i < points_per_face - x_points; i++) {
            glBegin(GL_LINES);
                glColor3f(1.0, 0.0, 0.0);
                glVertex3f(points[i + index][X], 
                           points[i + index][Y], 
                           points[i + index][Z]); 
                glVertex3f(points[i + index][X] + x,
                           points[i + index][Y] + y, 
                           points[i + index][Z] + z);
            glEnd();
        } 
	};
    
    /* draw_side_normals - draws normals for every face of the cube. */
	void draw_side_normals() 
    {
        int points_per_face = x_points * (y_points + 1);

        one_side_normals(0, 0, N_MAG, 0);
        one_side_normals(points_per_face, 0, (-1) * N_MAG, 0);
        one_side_normals(2 * points_per_face, N_MAG, 0, 0);
        one_side_normals(3 * points_per_face, (-1) * N_MAG, 0, 0);
        one_side_normals(4 * points_per_face, 0, 0, N_MAG);
        one_side_normals(5 * points_per_face, 0, 0, (-1) * N_MAG);
    };

    /** override functions implmented by Shape superclass **/
    void draw_top() {};
    void draw_bottom() {};
    void draw_top_normals() {};
    void draw_bottom_normals() {};
    void draw_top_center_normal() {};
    void draw_bottom_center_normal() {};
}; 

#endif
