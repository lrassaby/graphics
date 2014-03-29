#define NUM_OPENGL_LIGHTS 8

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <GL/glui.h>
#include "Shape.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Sphere.h"
#include "SceneParser.h"
#include "Camera.h"

using namespace std;

/** These are the live variables passed into GLUI ***/
int  isectOnly = 1;

int	 camRotU = 0;
int	 camRotV = 0;
int	 camRotW = 0;
int  viewAngle = 45;
float eyeX = 2;
float eyeY = 2;
float eyeZ = 2;
float lookX = -2;
float lookY = -2;
float lookZ = -2;

/** These are GLUI control panel objects ***/
int  main_window;
string filenamePath = "data/general/sphere.xml";
GLUI_EditText* filenameTextField = NULL;
GLubyte* pixels = NULL;
int pixelWidth = 0, pixelHeight = 0;
int screenWidth = 0, screenHeight = 0;

/** these are the global variables used for rendering **/
Cube* cube = new Cube();
Cylinder* cylinder = new Cylinder();
Cone* cone = new Cone();
Sphere* sphere = new Sphere();
SceneParser* parser = NULL;
Camera* camera = new Camera();


/******************************************************/
/* Added by Jayme and Louis */

/* data types */
struct RenderNode {
	Matrix modelView;
    std::vector<ScenePrimitive*> primitives;
};

/* global variables */
std::vector<RenderNode> nodes;
bool initialload = true;

/* function signatures */
Matrix calcRotate(Vector axis, double gamma);
void flatten(SceneNode *root, Matrix modelView);
Vector generateRay(int i, int j, Camera *camera);
Point convertToNormalizedFilm (Point p);
double Intersect(Point eyePointP, Vector rayV, ScenePrimitive *object);
double IntersectCube(Point eyePointP, Vector rayV);
double IntersectCylinder(Point eyePointP, Vector rayV);
double IntersectCone(Point eyePointP, Vector rayV);
double IntersectSphere(Point eyePointP, Vector rayV);



/******************************************************/

void setupCamera();
void updateCamera();

void setPixel(GLubyte* buf, int x, int y, int r, int g, int b) {
	buf[(y*pixelWidth + x) * 3 + 0] = (GLubyte)r;
	buf[(y*pixelWidth + x) * 3 + 1] = (GLubyte)g;
	buf[(y*pixelWidth + x) * 3 + 2] = (GLubyte)b;
}

void callback_start(int id) {
	cout << "start button clicked!" << endl;

	if (parser == NULL) {
		cout << "no scene loaded yet" << endl;
		return;
	}

	pixelWidth = screenWidth;
	pixelHeight = screenHeight;

	updateCamera();

	if (pixels != NULL) {
		delete pixels;
	}
	pixels = new GLubyte[pixelWidth  * pixelHeight * 3];
	memset(pixels, 0, pixelWidth  * pixelHeight * 3);

	cout << "(w, h): " << pixelWidth << ", " << pixelHeight << endl;

	if (initialload) {
		SceneNode *root = parser->getRootNode();
		flatten(root, Matrix());
		initialload = false;
	}

    Point eyePoint(0, 0, 0);
	for (int i = 0; i < pixelWidth; i++) {
		for (int j = 0; j < pixelHeight; j++) {
            Vector d = generateRay(i, j, camera);
            for (int k = 0; k < nodes.size(); k++) {
                Matrix inv_mv = invert(nodes[k].modelView);
                Vector d_obj = inv_mv * d;
                //cerr << d_obj[0] << ", " << d_obj[1] << ", " << d_obj[2] << endl;
                Point  p_obj = inv_mv * eyePoint; 
                cerr << "should be for each node" << std::endl;
                for (int l = 0; l < nodes[k].primitives.size(); l++) {
                    float t = Intersect(p_obj, d_obj, nodes[k].primitives[l]);
                    std::cerr << "should be for each prim, t = " << t << std::endl;
                    if (t > 0) {
                        setPixel(pixels, i, j, 255, 255, 255);
                    }
                }
            }
		}
	}
	glutPostRedisplay();
}

void callback_load(int id) {
	char curDirName [2048];
	if (filenameTextField == NULL) {
		return;
	}
	printf ("%s\n", filenameTextField->get_text());

	if (parser != NULL) {
		delete parser;
	}
	parser = new SceneParser (filenamePath);
	cout << "success? " << parser->parse() << endl;

	setupCamera();
}


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
	camera->SetScreenSize(x, y);

	screenWidth = x;
	screenHeight = y;

	glutPostRedisplay();
}


/***************************************** setupCamera() *****************/
void setupCamera()
{
	SceneCameraData cameraData;
	parser->getCameraData(cameraData);

	camera->Reset();
	camera->SetViewAngle(cameraData.heightAngle);
	if (cameraData.isDir == true) {
		camera->Orient(cameraData.pos, cameraData.look, cameraData.up);
	}
	else {
		camera->Orient(cameraData.pos, cameraData.lookAt, cameraData.up);
	}

	viewAngle = camera->GetViewAngle();
	Point eyeP = camera->GetEyePoint();
	Vector lookV = camera->GetLookVector();
	eyeX = eyeP[0];
	eyeY = eyeP[1];
	eyeZ = eyeP[2];
	lookX = lookV[0];
	lookY = lookV[1];
	lookZ = lookV[2];
	camRotU = 0;
	camRotV = 0;
	camRotW = 0;
	GLUI_Master.sync_live_all();
}

void updateCamera()
{
	camera->Reset();
    Vector up = camera->GetUpVector();

	Point guiEye (eyeX, eyeY, eyeZ);
	Point guiLook(lookX, lookY, lookZ);
	camera->SetViewAngle(viewAngle);
	camera->Orient(guiEye, guiLook, up);
	camera->RotateU(camRotU);
	camera->RotateV(camRotV);
	camera->RotateW(camRotW);
}

/***************************************** myGlutDisplay() *****************/

void myGlutDisplay(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (parser == NULL) {
		return;
	}

	if (pixels == NULL) {
		return;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDrawPixels(pixelWidth, pixelHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	glutSwapBuffers();
}

void onExit()
{
	delete cube;
	delete cylinder;
	delete cone;
	delete sphere;
	delete camera;
	if (parser != NULL) {
		delete parser;
	}
	if (pixels != NULL) {
		delete pixels;
	}
}

/**************************************** main() ********************/

int main(int argc, char* argv[])
{
    if (argc > 1) {
        filenamePath = argv[1];
    }
	atexit(onExit);

	/****************************************/
	/*   Initialize GLUT and create window  */
	/****************************************/

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(500, 500);

	main_window = glutCreateWindow("COMP 175 Assignment 4");
	glutDisplayFunc(myGlutDisplay);
	glutReshapeFunc(myGlutReshape);

	/****************************************/
	/*         Here's the GLUI code         */
	/****************************************/

	GLUI* glui = GLUI_Master.create_glui("GLUI");

	filenameTextField = new GLUI_EditText( glui, "Filename:", filenamePath);
	filenameTextField->set_w(300);
	glui->add_button("Load", 0, callback_load);
	glui->add_button("Start!", 0, callback_start);
	glui->add_checkbox("Isect Only", &isectOnly);
	
	GLUI_Panel *camera_panel = glui->add_panel("Camera");
	(new GLUI_Spinner(camera_panel, "RotateV:", &camRotV))
		->set_int_limits(-179, 179);
	(new GLUI_Spinner(camera_panel, "RotateU:", &camRotU))
		->set_int_limits(-179, 179);
	(new GLUI_Spinner(camera_panel, "RotateW:", &camRotW))
		->set_int_limits(-179, 179);
	(new GLUI_Spinner(camera_panel, "Angle:", &viewAngle))
		->set_int_limits(1, 179);

	glui->add_column_to_panel(camera_panel, true);

	GLUI_Spinner* eyex_widget = glui->add_spinner_to_panel(camera_panel, "EyeX:", GLUI_SPINNER_FLOAT, &eyeX);
	eyex_widget->set_float_limits(-10, 10);
	GLUI_Spinner* eyey_widget = glui->add_spinner_to_panel(camera_panel, "EyeY:", GLUI_SPINNER_FLOAT, &eyeY);
	eyey_widget->set_float_limits(-10, 10);
	GLUI_Spinner* eyez_widget = glui->add_spinner_to_panel(camera_panel, "EyeZ:", GLUI_SPINNER_FLOAT, &eyeZ);
	eyez_widget->set_float_limits(-10, 10);

	GLUI_Spinner* lookx_widget = glui->add_spinner_to_panel(camera_panel, "LookX:", GLUI_SPINNER_FLOAT, &lookX);
	lookx_widget->set_float_limits(-10, 10);
	GLUI_Spinner* looky_widget = glui->add_spinner_to_panel(camera_panel, "LookY:", GLUI_SPINNER_FLOAT, &lookY);
	looky_widget->set_float_limits(-10, 10);
	GLUI_Spinner* lookz_widget = glui->add_spinner_to_panel(camera_panel, "LookZ:", GLUI_SPINNER_FLOAT, &lookZ);
	lookz_widget->set_float_limits(-10, 10);

	glui->add_button("Quit", 0, (GLUI_Update_CB)exit);

	glui->set_main_gfx_window(main_window);

	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(myGlutIdle);

	glutMainLoop();

	return EXIT_SUCCESS;
}

/* from Assignment 3 */

/* flatten - parse SceneNode tree and flatten into a RenderNode vector */
void flatten(SceneNode *root, Matrix modelView)
{
    RenderNode node;
    Matrix transmat;
    SceneTransformation *trans;
    int size = root->transformations.size();
    for (int i = 0; i < size; i++) {
    	trans = root->transformations[i];
    	switch (trans->type) {
    		case TRANSFORMATION_TRANSLATE:
    			transmat = 
	    		Matrix(1, 0, 0, trans->translate[X],
	    			   0, 1, 0, trans->translate[Y],
	    			   0, 0, 1, trans->translate[Z],
	    			   0, 0, 0, 1);
	    		modelView = modelView * transmat;
	    		break;
    		case TRANSFORMATION_ROTATE:
    			transmat = calcRotate(trans->rotate, trans->angle);
    			modelView = modelView * transmat;
    			break;
    		case TRANSFORMATION_SCALE:
	    		transmat =
	    		Matrix(trans->scale[X], 0, 0, 0,
	    			   0, trans->scale[Y], 0, 0,
	    			   0, 0, trans->scale[Z], 0,
	    			   0, 0, 0, 1);
	    		modelView = modelView * transmat;
    			break;
    		case TRANSFORMATION_MATRIX:
    			transmat = trans->matrix;
    			modelView = modelView * transmat;
    			break;
    	}
    }
    node.primitives = root->primitives;
    node.modelView = modelView;
    nodes.push_back(node);

    for (int i = 0; i < root->children.size(); i++) {
        flatten(root->children[i], modelView);
    }
}

/* calcRotate - an arbitrary rotation around origin */
Matrix calcRotate(Vector axis, double gamma) 
{
    double theta = atan2(axis[Z], axis[X]);
    double phi = -atan2(axis[Y], sqrt(axis[X] * axis[X] + axis[Z] * axis[Z]));
    Matrix M1, M2, M3, M1_inv, M2_inv;
    M1 = rotY_mat(theta); 
    M2 = rotZ_mat(phi); 
    M3 = rotX_mat(gamma);
    M1_inv = inv_rotY_mat(theta);
    M2_inv = inv_rotZ_mat(phi);

    return M1_inv * M2_inv * M3 * M2 * M1;
}

/*
 ******************************************************************* 
 *                    Louis and Jayme's Assignment 4 code 
 *******************************************************************
 */

Vector generateRay(int i, int j, Camera *camera)
{
    Vector d;
    Point p_world(i, j, -1);
    Point eyePoint(0, 0, 0);

    /* convert p_world to normalized film space */
    Point p_camera = convertToNormalizedFilm(p_world);

    /* convert p to camera space */
    p_camera = camera->GetInverseTransformMatrix() * p_camera; 

    d = (p_camera - eyePoint);
    return normalize(d);
}

Point convertToNormalizedFilm(Point p) 
{
    return Point ((2 * p[X] / screenWidth) - 1, 1 - (2 * p[Y] / screenHeight), -1);
}


double Intersect(Point eyePointP, Vector rayV, ScenePrimitive *object)
{
    switch (object->type) {
        case SHAPE_CUBE:
            return IntersectCube(eyePointP, rayV);
            break;
        case SHAPE_CYLINDER:
            return IntersectCylinder(eyePointP, rayV);
            break;
        case SHAPE_CONE:
            return IntersectCone(eyePointP, rayV);
            break;
        case SHAPE_SPHERE:
            return IntersectSphere(eyePointP, rayV);
            break;
        default:
            cerr << "Unrecognized shape selected." << endl;
            return -1;
            break;
    }
}

double IntersectCube(Point eyePointP, Vector rayV) 
{ 
    return 0;
}

double IntersectCylinder(Point eyePointP, Vector rayV) 
{
    return 0;
}

double IntersectCone(Point eyePointP, Vector rayV)
{
    return 0;
}

double IntersectSphere(Point eyePointP, Vector rayV) {
    double A, B, C; 
    Vector eyeVector = eyePointP - Point(0, 0, 0);

    A = dot(rayV, rayV);
    B = 2 * dot(eyeVector, rayV);
    C = dot(eyeVector, eyeVector) - 1; 


    float determinant = B * B - 4 * A * C; 
    if (determinant < 0) {
        cerr << "No intersect" << endl;
        return 0;
    } else if (determinant == 0) {
        cerr << "One intersection at " << -1 * B/ (2 * A) << endl;
        return -1 * B / (2* A);
    } else {
        cerr << "Two intersections at " << (-1 * B + sqrt(determinant)) / (2 * A)
                  << " and "                 << (-1 * B - sqrt(determinant)) / (2 * A) << endl;
        return std::min((-1 * B - sqrt(determinant)) / (2 * A), 
                        (-1 * B + sqrt(determinant)) / (2 * A));
    }
}
