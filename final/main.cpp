/*
 * Jayme Woogerd and Louis Rassaby
 * Comp 175 - final project
 * April 18, 2014
 *
 * main.cpp 
 */

#include <string.h>
#include <iostream>
#include <fstream>
#include <GL/glui.h>
#include <math.h>
#include "fountain.h"

/* These are the live variables passed into GLUI */
int main_window;
int rotX = 0, rotY = 0, rotZ = 0;
int scale = 100;

/* global variables */
Fountain fountain;

void myGlutIdle(void)
{
	/* According to the GLUT specification, the current window is
	undefined during an idle callback.  So we need to explicitly change
	it if necessary */
	if (glutGetWindow() != main_window){
		glutSetWindow(main_window);
	}

	glutPostRedisplay();
}

/*
	We configure our window so that it correctly displays our objects
	in the correct perspective.
*/
void myGlutReshape(int x, int y)
{
	float xy_aspect;
	xy_aspect = (float)x / (float)y;
	// TODO: glutFullScreen();
	//
	glViewport(0, 0, x, y);
	// Determine if we are modifying the camera(GL_PROJECITON) matrix(which is our viewing volume)
	// Otherwise we could modify the object transormations in our world with GL_MODELVIEW
	glMatrixMode(GL_PROJECTION);
	// Reset the Projection matrix to an identity matrix
	glLoadIdentity();
	// The frustrum defines the perspective matrix and produces a perspective projection.
	// It works by multiplying the current matrix(in this case, our matrix is the GL_PROJECTION)
	// and multiplies it.
	glFrustum(-xy_aspect*.08, xy_aspect*.08, -.08, .08, .1, 15.0);
	// Since we are in projection mode, here we are setting the camera to the origin (0,0,0)
	glTranslatef(0, 0, 0);
	// Call our display function.
	glutPostRedisplay();
}

/*
void drawAxis(){
	glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0, 0, 0); glVertex3f(1.0, 0, 0);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0, 0, 0); glVertex3f(0.0, 1.0, 0);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0, 0, 0); glVertex3f(0, 0, 1.0);
	glEnd();
}
*/

void drawAxis(){                                                                    
    glBegin(GL_LINES);                                                              
        glColor3f(1.0, 0.0, 0.0);                                                   
        glVertex3f(0, 0, 0); glVertex3f(1.0, 0, 0);                                 
        glColor3f(0.0, 1.0, 0.0);                                                   
        glVertex3f(0, 0, 0); glVertex3f(0.0, 1.0, 0);                               
        glColor3f(0.0, 0.0, 1.0);                                                   
        glVertex3f(0, 0, 0); glVertex3f(0, 0, 1.0);                                 
    glEnd();                                                                        
}    

void myGlutDisplay(void)
{
	// Clear the buffer of colors in each bit plane.
	// bit plane - A set of bits that are on or off (Think of a black and white image)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// here we are moving the camera back by 0.5 on the z-axis
	// TODO: Implement translation, scaling, rotation
	
	glTranslatef(0, 0, -0.5);
	
	//allow for user controlled rotation and scaling
	glScalef(scale / 100.0, scale / 100.0, scale / 100.0);
	glRotatef(rotX, rotY, rotZ, 0.0);
	
#if 0
    glLoadIdentity();
    glRotatef(50.0, 1.0, 0.0, 0.0);
#endif 
	// In this case, just the drawing of the axes.
	drawAxis();

	// draw the fountain
	fountain.drawParticles();
	
	// Pop all of these operations off of our model_view stack, thus getting us back
	// to the default state.

	glutSwapBuffers();
}

/*  ==========================================
	Clean up all dynamically allocated memory
	========================================== */
void onExit()
{
}


int main(int argc, char* argv[])
{
	
	atexit(onExit);

	/****************************************/
	/*   Initialize GLUT and create window  */
	/****************************************/

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(500, 500);

	main_window = glutCreateWindow("COMP 175 In Class Assignment 1");
	glutDisplayFunc(myGlutDisplay);
	glutReshapeFunc(myGlutReshape);

	/****************************************/
	/*       Set up OpenGL lighting         */
	/****************************************/

	// Essentially set the background color of the 3D scene.
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glShadeModel(GL_FLAT);

	GLfloat light_pos0[] = { 0.0f, 0.0f, 1.0f, 0.0f };
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 0.0f };
	GLfloat ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);

	//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	/****************************************/
	/*          Enable z-buferring          */
	/****************************************/

	glEnable(GL_DEPTH_TEST);
	glPolygonOffset(1, 1);

	/****************************************/
	/*         Here's the GLUI code         */
	/****************************************/

	GLUI *glui = GLUI_Master.create_glui("GLUI");
	GLUI_Panel *camera_panel = glui->add_panel("Camera");
	(new GLUI_Spinner(camera_panel, "Rotate X:", &rotX))
		->set_int_limits(0, 359);
	(new GLUI_Spinner(camera_panel, "Rotate Y:", &rotY))
		->set_int_limits(0, 359);
	(new GLUI_Spinner(camera_panel, "Rotate Z:", &rotZ))
		->set_int_limits(0, 359);
	(new GLUI_Spinner(camera_panel, "Scale:", &scale))
		->set_int_limits(1, 1000);


	glui->add_column(true);
	glui->add_button("Quit", 0, (GLUI_Update_CB)exit);

	glui->set_main_gfx_window(main_window);
	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(myGlutIdle);



	glutMainLoop();

	return EXIT_SUCCESS;
}


