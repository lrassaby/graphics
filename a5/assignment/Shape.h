#ifndef SHAPE_H
#define SHAPE_H

#include <GL/glui.h>
#include "Algebra.h"

#define HEIGHT 1.0
#define SCALE 0.5
#define N_MAG 0.1
#define X 0
#define Y 1
#define Z 2

class Shape {

public:
	Shape() 
    {
        x_segments = y_segments = 0;
        top = Point(0, SCALE, 0);
        bottom = Point(0, (-1.0 * SCALE), 0);
        points = NULL;
    };

	~Shape() { delete [] points; };

	void setSegments(int x, int y) {
		m_segmentsX = x;
		m_segmentsY = y;
	}

	virtual void draw() 
    {
        if (x_segments != m_segmentsX || y_segments != m_segmentsY) {
            x_segments = m_segmentsX;
            y_segments = m_segmentsY;
            x_points = x_segments + 1;
            y_points = y_segments + 1;
            calculate_points();
        }
        draw_top();
        draw_bottom();
        draw_sides();
    };
    
    virtual void drawNormal() 
    {
        glColor3f(1.0, 0.0, 0.0);
        draw_top_center_normal();
        draw_bottom_center_normal();
        draw_top_normals();
        draw_bottom_normals();
        draw_side_normals();
    };

    /* 
     * draw_top - draws the top of the shape; in most cases this will be the 
     * top ring of points to the top center point.
     */
    virtual void draw_top()
    {
        for (int i = 0; i < x_segments; i++) {
            Vector n = cross(points[i] - top, points[i] - points[i + 1]);
            n.normalize();
            glBegin(GL_TRIANGLES);
                glNormal3f(n[X], n[Y], n[Z]);
                glVertex3f(points[i][X], 
                           points[i][Y], 
                           points[i][Z]);
                glNormal3f(n[X], n[Y], n[Z]);
                glVertex3f(top[X], top[Y], top[Z]); 
                glNormal3f(n[X], n[Y], n[Z]);
                glVertex3f(points[i + 1][X], 
                           points[i + 1][Y], 
                           points[i + 1][Z]);
            glEnd();
        }
    }

    /* 
     * draw_bottom - draws the bottom of the shape; in most cases this will be
     * the bottom ring of points to bottom center point.
     */
    virtual void draw_bottom()
    {
        int start = num_points - x_points;
        for (int i = start; i < num_points - 1; i++) {
            Vector n = cross(points[i] - points[i + 1], points[i] - bottom);
            n.normalize();
            glBegin(GL_TRIANGLES);
                glNormal3f(n[X], n[Y], n[Z]);
                glVertex3f(points[i][X], 
                           points[i][Y], 
                           points[i][Z]);
                glNormal3f(n[X], n[Y], n[Z]);
                glVertex3f(bottom[X], bottom[Y], bottom[Z]); 
                glNormal3f(n[X], n[Y], n[Z]);
                glVertex3f(points[i + 1][X], 
                           points[i + 1][Y], 
                           points[i + 1][Z]);
            glEnd();
        }
    }

    /* draw_sides - draws the sides of the shape, minus the top and bottom. */ 
    virtual void draw_sides()
    {
        int limit =  num_points - x_points;
        for (int i = 0; i < limit; i++) {
            if (i % x_points != x_segments) {        
                Vector n = cross(points[i] - points[i + 1],
                                 points[i] - points[i + x_points]);
                n.normalize();
                glBegin(GL_TRIANGLES);
                    glNormal3f(n[X], n[Y], n[Z]);
                    glVertex3f(points[i][X], 
                               points[i][Y], 
                               points[i][Z]); 
                    glNormal3f(n[X], n[Y], n[Z]);
                    glVertex3f(points[i + x_points][X], 
                               points[i + x_points][Y], 
                               points[i + x_points][Z]);
                    glNormal3f(n[X], n[Y], n[Z]);
                    glVertex3f(points[i + 1][X], 
                               points[i + 1][Y], 
                               points[i + 1][Z]);
                glEnd();

                glBegin(GL_TRIANGLES);
                    glNormal3f(n[X], n[Y], n[Z]);
                    glVertex3f(points[i + x_points][X], 
                               points[i + x_points][Y], 
                               points[i + x_points][Z]);
                    glNormal3f(n[X], n[Y], n[Z]);
                    glVertex3f(points[i + x_points + 1][X], 
                               points[i + x_points + 1][Y], 
                               points[i + x_points + 1][Z]);
                    glNormal3f(n[X], n[Y], n[Z]);
                    glVertex3f(points[i + 1][X], 
                               points[i + 1][Y], 
                               points[i + 1][Z]); 
                glEnd();
            }
        }
    }

    /** functions for drawing normals **/

    /* 
     * draw_top_center and draw_bottom_center_normals - draw one normal at the 
     * center of the top and bottom faces in +/- y direction, respectively.
     */
    virtual void draw_top_center_normal()
    {
         glBegin(GL_LINES);
            glVertex3f(top[X], top[Y], top[Z]);
            glVertex3f(top[X], top[Y] + N_MAG, top[Z]);
        glEnd();
    }

    virtual void draw_bottom_center_normal()
    {
         glBegin(GL_LINES);
            glVertex3f(bottom[X], bottom[Y], bottom[Z]);
            glVertex3f(bottom[X], bottom[Y] - N_MAG, bottom[Z]);
        glEnd();
    }

    /* 
     * draw_top and draw_bottom_normals - draw normals for the top and bottom
     * faces, respectively.
     */
    virtual void draw_top_normals()
    {
         for (int i = 0; i < x_segments; i++) {
            glBegin(GL_LINES);
                glVertex3f(points[i][X], 
                           points[i][Y], 
                           points[i][Z]); 
                glVertex3f(points[i][X], 
                           points[i][Y] + N_MAG, 
                           points[i][Z]);
            glEnd();
        }
    }

    virtual void draw_bottom_normals()
    {
        for (int i = num_points - x_points; i < num_points - 1; i++) {
            glBegin(GL_LINES);
                glVertex3f(points[i][X], 
                           points[i][Y], 
                           points[i][Z]); 
                glVertex3f(points[i][X], 
                           points[i][Y] - N_MAG, 
                           points[i][Z]);
            glEnd(); 
        }
    }
    
    /** functions implemented by each subclass shape **/
    virtual void draw_side_normals() {};
    virtual void calculate_points() {};

protected:
    
	void normalizeNormal (float x, float y, float z)
    {
		normalizeNormal (Vector(x, y, z));
	};

	void normalizeNormal (Vector v) 
    {
		v.normalize();
		glNormal3dv(v.unpack());
	};

	int m_segmentsX, m_segmentsY;
    int x_segments;     /* segments in the radial direction */
    int y_segments;     /* segments in the y direction */
    int x_points;       /* points in the radial direction; x_segments + 1 */
    int y_points;       /* points in the y direction; y_segments + 1 */ 

    Point top;          /* top center point at (0, SCALE, 0) */
    Point bottom;       /* bottom center point at (0, (-1.0 * SCALE), 0) */
    Point *points;      /* represent each shape as array of points */
    int num_points;     /* # of total points need to draw shape */
};

#endif
