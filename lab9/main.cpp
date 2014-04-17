/*  =================== File Information =================
	File Name: main.cpp
	Description:
	Author: Michael Shah
	Last Modified: 4/14/14

	Purpose: 
	Usage:	

	Further Reading resources: 
	===================================================== */

#include <string.h>
#include <iostream>
#include <fstream>
// You may need to include glee.h here if you are having problems
#include <GL/glui.h>
#include <math.h>
#include "shaderManager.h"

#define PI 3.14

/** These are the live variables passed into GLUI ***/
int main_window;
int wireframe = 0;
int filled = 1;
int grid = 0;

float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float obj_pos[] = { 0.0, 0.0, 0.0 };

float xLightPos = 0.0;
float yLightPos = 0.0;
float zLightPos = 0.0;


// Pointer to widget
GLUI *glui2;


/* 	======== Shader Manager ========
	Setup a new shader manager
	============================== */
shaderManager* myShaderManager;


/***************************************** myGlutIdle() ***********/

void myGlutIdle(void)
{
	/* According to the GLUT specification, the current window is
	undefined during an idle callback.  So we need to explicitly change
	it if necessary */
	if (glutGetWindow() != main_window)
		glutSetWindow(main_window);

	glutPostRedisplay();
}


/**************************************** myGlutReshape() *************/

void myGlutReshape(int x, int y)
{
	float xy_aspect;

	xy_aspect = (float)x / (float)y;
	glViewport(0, 0, x, y);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-xy_aspect*.08, xy_aspect*.08, -.08, .08, .1, 15.0);
	glTranslatef(0, 0, 0);

	glutPostRedisplay();
}



void draw_grid(){
	int grid_size = 10;
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(-grid_size/2,0,-grid_size/2);
    // Draw a grid under the object
        for(int i = 0 ; i < grid_size; i=i+1){
            glBegin(GL_LINES);
                glColor3f( 1.0, 1.0, 1.0 );
                glVertex3f( 0.0, 0.0, i );  glVertex3f( grid_size, 0.0, i ); /* X axis grid */
                glVertex3f( i, 0.0, 0.0 );  glVertex3f( i, 0.0, grid_size ); /* X axis grid */
            glEnd();
        }
    
        glBegin(GL_LINES);
            glColor3f( 1.0, 1.0, 1.0 );
            glVertex3f( 0.0, 0.0, grid_size );  glVertex3f( grid_size, 0.0, grid_size ); /* X axis grid */
            glVertex3f( grid_size, 0.0, 0.0 );  glVertex3f( grid_size, 0.0, grid_size ); /* X axis grid */
        glEnd();
    glPopMatrix();
    glEnable( GL_LIGHTING );

}

/***************************************** myGlutDisplay() *****************/

// Draw a triangle that represents the light source
void drawLightSource(){
	xLightPos += 0.001f;
	yLightPos = 0.5;
	zLightPos += 0.001f;

	// Actual values that we calculate and send to the shader
	float xLightValue = sin(xLightPos);
	float yLightValue = yLightPos;
	float zLighValue = cos(zLightPos)+1;

	// Associate our uniform value 'light Vector' in our shader with these 3 values
	// the '3f' at the end means 'lightVector' is a vector of floats
	glUniform3f(glGetUniformLocation(myShaderManager->program,"lightVector"),xLightValue,yLightValue,zLighValue);

	glPushMatrix();
		glTranslatef(xLightValue,yLightValue,zLighValue);
		glColor3f(0.7,0.7,0);
		glBegin(GL_QUADS);
			glVertex3f(-.1,.1,0);
			glVertex3f(-.1,-.1,0);
			glVertex3f(.1,-.1,0);
			glVertex3f(.1,.1,0);
		glEnd();
	glPopMatrix();
}


void drawQuad(){


	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	// Pass information to our shader to our shader
		int texture_location = glGetUniformLocation(myShaderManager->program,"brick_image");
		glUniform1i(texture_location,0);
	glBindTexture(GL_TEXTURE_2D,myShaderManager->layer1ID);

	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
		glUniform1i(glGetUniformLocation(myShaderManager->program,"bump_image"),1);
	glBindTexture(GL_TEXTURE_2D,myShaderManager->layer2ID);

	// Pass in the shader program id, which is 'program'		

	glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex3f(-1,1,0);
		glTexCoord2f(1,0);
		glVertex3f(-1,-1,0);
		glTexCoord2f(1,1);
		glVertex3f(1,-1,0);
		glTexCoord2f(0,1);
		glVertex3f(1,1,0);
	glEnd();

	glActiveTexture(GL_TEXTURE1);  
	glBindTexture(GL_TEXTURE_2D, 0);  
	glDisable(GL_TEXTURE_2D);  
	  
	glActiveTexture(GL_TEXTURE0);  
	glBindTexture(GL_TEXTURE_2D, 0);  
	glDisable(GL_TEXTURE_2D);

	drawLightSource();

}

void myGlutDisplay(void)
{

	static float scale = 0.1;
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glScalef(scale,scale,scale);
	glTranslatef( obj_pos[0], obj_pos[1]-2, -obj_pos[2]-6 );
	glMultMatrixf(view_rotate);

		if (filled) {
			glEnable(GL_POLYGON_OFFSET_FILL);
			glColor3f(0.5, 0.5, 0.5);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			//mySolarSystem->render();
			drawQuad();
		}
		
		if (wireframe) {
			glDisable(GL_POLYGON_OFFSET_FILL);
			glColor3f(0.0, 0.0, 0.0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glPopMatrix();
			//mySolarSystem->render();
			drawQuad();
		}

		if(grid){
			draw_grid();
		}

	glutSwapBuffers();
}

/*
	Reclaim memory we've allocated
*/
void onExit()
{
	delete myShaderManager;
}



/**************************************** main() ********************/

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

	main_window = glutCreateWindow("COMP 175 In Class Assignment 9");
	glutDisplayFunc(myGlutDisplay);
	glutReshapeFunc(myGlutReshape);

	/****************************************/
	/*       Set up OpenGL lighting         */
	/****************************************/

	glShadeModel(GL_SMOOTH);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	static float one[] = { 1, 1, 1, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, one);


	/****************************************/
	/*       Set up OpenGL Shaders          */
	/****************************************/

	// Initialize our shader using our special shader manager class.
	myShaderManager = new shaderManager();
	
	myShaderManager->loadImage("brick.ppm",0);
	myShaderManager->loadImage("bump.ppm",1);

	myShaderManager->initShader("shaders/Bump/bump.vert","shaders/Bump/bump.frag");

	/****************************************/
	/*          Enable z-buferring          */
	/****************************************/

	glEnable(GL_DEPTH_TEST);
	glPolygonOffset(1, 1);

	/****************************************/
	/*         Here's the GLUI code         */
	/****************************************/

	GLUI *glui = GLUI_Master.create_glui("GLUI");

	/*** Create the bottom subwindow ***/
    glui2 = GLUI_Master.create_glui_subwindow( main_window, GLUI_SUBWINDOW_BOTTOM );
    // Create a rotation widget
    GLUI_Rotation *view_rot = new GLUI_Rotation(glui2, "Objects", view_rotate );
    view_rot->set_spin( 1.0 );

    // Navigate our world
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_x =  new GLUI_Translation(glui2, "Objects X", GLUI_TRANSLATION_X, obj_pos );
    trans_x->set_speed( .1 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_y =  new GLUI_Translation( glui2, "Objects Y", GLUI_TRANSLATION_Y, &obj_pos[1] );
    trans_y->set_speed( .1 );
    new GLUI_Column( glui2, false );
    GLUI_Translation *trans_z =  new GLUI_Translation( glui2, "Objects Z", GLUI_TRANSLATION_Z, &obj_pos[2] );
    trans_z->set_speed( .1 );

	glui2->add_column(true);

	GLUI_Panel *render_panel = glui2->add_panel("Render");
	new GLUI_Checkbox(render_panel, "Wireframe", &wireframe);
	new GLUI_Checkbox(render_panel, "Filled", &filled);
	new GLUI_Checkbox(render_panel, "Grid", &grid);


	glui2->add_button("Quit", 0, (GLUI_Update_CB)exit);



	glui->set_main_gfx_window(main_window);
	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(myGlutIdle);



	glutMainLoop();

	return EXIT_SUCCESS;
}



