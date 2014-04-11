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
#define NO_INTERSECT -1
#define DENOMINATOR 255

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
int recursive_depth = 0;

/** These are GLUI control panel objects ***/
int  main_window;
string filenamePath = "/Users/louisrassaby/Dropbox/classes/comp175/part2/a4/assignment/data/general/ball.xml";
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

enum colors {R, G, B};

struct RaycastObject { /* a point on a shape */
    double t;
    ScenePrimitive *shape;
    Point ps_obj;
    Point ps_world;
    Point eye_world;
    Matrix object_to_world;
    Matrix world_to_object;
};

/* global variables */
std::vector<RenderNode> nodes;
bool initialload = true;

/* function signatures */
Matrix calcRotate(Vector axis, double gamma);
void flatten(SceneNode *root, Matrix modelView);
Vector generateRay(int i, int j);
Point convertToNormalizedFilm (Point p);
double Intersect(Point eyePointP, Vector rayV, ScenePrimitive *object);
double IntersectCube(Point eyePointP, Vector rayV);
double IntersectCylinder(Point eyePointP, Vector rayV);
double IntersectCone(Point eyePointP, Vector rayV);
double IntersectSphere(Point eyePointP, Vector rayV);
double minPositive(double *values, int length);
double quadraticIntersect(double A, double B, double C);
double calculateIntensity (RaycastObject *obj, int channel, int depth);
bool isOutOfBounds(double min, double max, double tocheck) {
    return (tocheck < min || tocheck > max);
};
std::vector<RaycastObject> findIntersections(Vector d_world, Point origin_world);

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

	for (int i = 0; i < pixelWidth; i++) {
		for (int j = 0; j < pixelHeight; j++) {
            Vector d = generateRay(i, j);
            Vector d_world = camera->GetInverseTransformMatrix() * d;
            d_world.normalize();

            std::vector<RaycastObject> t_objects = findIntersections(d_world, camera->GetEyePoint());
            if (isectOnly) {
                if (!t_objects.empty()) {
                    setPixel(pixels, i, j, 255, 255, 255);
                }
            } else {
                int len = t_objects.size();
                if (len > 0) { /* if there is an intersection */
                    RaycastObject first_obj = t_objects[0];
                    for (int k = 1; k < len; k++) {
                        if (t_objects[k].t < first_obj.t) {
                            first_obj = t_objects[k];
                        }
                    }
                    t_objects.clear();

                    setPixel(pixels, i, j,
                            calculateIntensity(&first_obj, R, recursive_depth) * DENOMINATOR, 
                            calculateIntensity(&first_obj, G, recursive_depth) * DENOMINATOR,
                            calculateIntensity(&first_obj, B, recursive_depth) * DENOMINATOR);
                } /* else no intersect */
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
    nodes.clear();
    initialload = true;

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
    nodes.clear();
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

	main_window = glutCreateWindow("COMP 175 Assignment 5");
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

    GLUI_Panel *raytracer_panel = glui->add_panel("Recursive Raytracer");
    (new GLUI_Spinner(raytracer_panel, "Recursive Depth:", &recursive_depth))
        ->set_int_limits(0, 10);


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

Vector generateRay(int i, int j)
{
    Vector d;
    Point p(i, j, -1);
    Point eyePoint(0, 0, 0);

    /* convert p to normalized film space, p_film */
    Point p_film = convertToNormalizedFilm(p);

    d = (p_film  - eyePoint);
    return d; 
}

/*
 * convertToNormalizedFilm - converts a point in pixel space 
 * to a point in the normalized film space.
 */
Point convertToNormalizedFilm(Point p) 
{
    return Point ((2.0 * p[X] / screenWidth) - 1.0, 
            (2.0 * p[Y] / screenWidth) - 1.0, -1.0);
}

/* 
 * Intersect - give the eyepoint, unit ray vector in object 
 * space and the object, returns the closest intersection point, t.
 */
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

/* 
 * IntersectCube - give the eyepoint and unit ray vector in object 
 * space, returns the closest intersection point, t, for a cube.
 */
double IntersectCube(Point eyePointP, Vector rayV) 
{ 
    int num_faces = 6;
    double components[num_faces];
    Point intersect; 
    if (rayV[X] != 0) {
        /* x == 0.5 plane */
        components[0] = (0.5 - eyePointP[X]) / rayV[X];
        intersect = eyePointP + components[0] * rayV; 
        if (isOutOfBounds(-0.5, 0.5, intersect[Y]) || 
            isOutOfBounds(-0.5, 0.5, intersect[Z])) {
            components[0] = NO_INTERSECT;
        }
        /* x == -0.5 plane */
        components[1] = (-0.5 - eyePointP[X]) / rayV[X];
        intersect = eyePointP + components[1] * rayV; 
        if (isOutOfBounds(-0.5, 0.5, intersect[Y]) || 
            isOutOfBounds(-0.5, 0.5, intersect[Z])) {
            components[1] = NO_INTERSECT;
        }
    } else {
        components[0] = NO_INTERSECT;
        components[1] = NO_INTERSECT;
    }

    if (rayV[Y] != 0) {
        /* y == 0.5 plane */
        components[2] = (0.5 - eyePointP[Y]) / rayV[Y];
        intersect = eyePointP + components[2] * rayV; 
        if (isOutOfBounds(-0.5, 0.5, intersect[X]) || 
            isOutOfBounds(-0.5, 0.5, intersect[Z])) {
            components[2] = NO_INTERSECT;
        }
        /* y == -0.5 plane */
        components[3] = (-0.5 - eyePointP[Y]) / rayV[Y];
        intersect = eyePointP + components[3] * rayV; 
        if (isOutOfBounds(-0.5, 0.5, intersect[X]) || 
            isOutOfBounds(-0.5, 0.5, intersect[Z])) {
            components[3] = NO_INTERSECT;
        }
    } else {
        components[2] = NO_INTERSECT;
        components[3] = NO_INTERSECT;
    }
    if (rayV[Z] != 0) {
        /* z == 0.5 plane */
        components[4] = (0.5 - eyePointP[Z]) / rayV[Z];
        intersect = eyePointP + components[4] * rayV; 
        if (isOutOfBounds(-0.5, 0.5, intersect[X]) || 
            isOutOfBounds(-0.5, 0.5, intersect[Y])) {
            components[4] = NO_INTERSECT;
        }
        /* z == -0.5 plane */
        components[5] = (-0.5 - eyePointP[Z]) / rayV[Z];
        intersect = eyePointP + components[5] * rayV; 
        if (isOutOfBounds(-0.5, 0.5, intersect[X]) || 
            isOutOfBounds(-0.5, 0.5, intersect[Y])) {
            components[5] = NO_INTERSECT;
        }
    } else {
        components[4] = NO_INTERSECT;
        components[5] = NO_INTERSECT;
    }
    return minPositive(components, num_faces);
}

#define T_BODY 0
#define T_BOTTOM 1
#define T_TOP 2
/* 
 * IntersectCylinder - give the eyepoint and unit ray vector in object 
 * space, returns the closest intersection point, t, for a cylinder.
 */
double IntersectCylinder(Point eyePointP, Vector rayV) 
{
    Point intersect; 
    double components[3];
    double A, B, C;
    Point objCenter(0, 0, 0);
    Vector eyeVector = eyePointP - objCenter;

    A = SQR(rayV[X]) + SQR(rayV[Z]);
    B = 2 * (eyePointP[X] * rayV[X] + eyePointP[Z] * rayV[Z]);
    C = SQR(eyePointP[X]) + SQR(eyePointP[Z]) - .25;

    components[T_BODY] = quadraticIntersect(A, B, C);
    intersect = eyePointP + components[T_BODY] * rayV; 
    if (isOutOfBounds(-0.5, 0.5, intersect[Y])) {
        components[T_BODY] = NO_INTERSECT;
    }

    if (rayV[Y] != 0) {
        components[T_TOP] = (0.5 - eyePointP[Y]) / rayV[Y];
        intersect = eyePointP + components[T_TOP] * rayV; 
        if (isOutOfBounds(0, 0.25, SQR(intersect[X]) + SQR(intersect[Z]))) {
            components[T_TOP] = NO_INTERSECT;
        }
        components[T_BOTTOM] = (-0.5 - eyePointP[Y]) / rayV[Y];
        intersect = eyePointP + components[T_BOTTOM] * rayV; 
        if (isOutOfBounds(0, 0.25, SQR(intersect[X]) + SQR(intersect[Z]))) {
            components[T_BOTTOM] = NO_INTERSECT;
        }
    } else {
        components[T_BOTTOM] = NO_INTERSECT;
        components[T_TOP] = NO_INTERSECT;
    }

    return minPositive(components, 3);
}
#undef T_BODY
#undef T_BOTTOM
#undef T_TOP

#define T_BODY 0
#define T_BASE 1
/* 
 * IntersectCone - give the eyepoint and unit ray vector in object 
 * space, returns the closest intersection point, t, for a cone.
 */
double IntersectCone(Point eyePointP, Vector rayV)
{
    Point intersect; 
    double components[2];
    double A, B, C;
    Point objCenter(0, 0, 0);
    Vector eyeVector = eyePointP - objCenter;

    A = SQR(rayV[X]) + SQR(rayV[Z]) - 0.25 * SQR(rayV[Y]);
    B = (2.0 * eyePointP[X] * rayV[X]) + 
        (2.0 * eyePointP[Z] * rayV[Z]) - 
        (0.5 * eyePointP[Y] * rayV[Y]) + 
        0.25 * rayV[Y];
    C = SQR(eyePointP[X]) + SQR(eyePointP[Z]) - 
        0.25 * SQR(eyePointP[Y]) + 0.25 * eyePointP[Y] - 0.0625;

    components[T_BODY] = quadraticIntersect(A, B, C);
    intersect = eyePointP + components[T_BODY] * rayV; 
    if (isOutOfBounds(-0.5, 0.5, intersect[Y])) {
        components[T_BODY] = NO_INTERSECT;
    }
    if (rayV[Y] != 0) {
        components[T_BASE] = (-0.5 - eyePointP[Y]) / rayV[Y];
        intersect = eyePointP + components[T_BASE] * rayV; 
        if (isOutOfBounds(0, 0.25, SQR(intersect[X]) + SQR(intersect[Z]))) {
            components[T_BASE] = NO_INTERSECT;
        }
    } else {
        components[T_BASE] = NO_INTERSECT;
    }
    return minPositive(components, 2);
}
#undef T_BODY
#undef T_BASE

/* 
 * IntersectSphere - give the eyepoint and unit ray vector in object 
 * space, returns the closest intersection point, t, for a sphere.
 */
double IntersectSphere(Point eyePointP, Vector rayV) {
    double A, B, C; 
    Point objCenter(0, 0, 0);
    Vector eyeVector = eyePointP - objCenter;

    A = dot(rayV, rayV);
    B = 2 * dot(eyeVector, rayV);
    C = dot(eyeVector, eyeVector) - 0.25; 

    return quadraticIntersect(A, B, C);

}

/* 
 * quadraticIntersect - returns the smallest positive solution to the
 * quadradic equation, given coefficients A, B and C.
 */
double quadraticIntersect(double A, double B, double C) 
{
    float determinant = B * B - 4.0 * A * C; 
    if (determinant < 0) {
        return NO_INTERSECT;
    } else if (determinant == 0) {
        return -1 * B / (2.0 * A);
    } else {
        double solutions[2];
        solutions[0] = (-1 * B - sqrt(determinant)) / (2.0 * A);
        solutions[1] = (-1 * B + sqrt(determinant)) / (2.0 * A);
        return minPositive(solutions, 2);
    }
}

 /* minPositive - returns the least positive value from values */
double minPositive(double *values, int length)
{
    double min_pos = NO_INTERSECT;
    for (int i = 0; i < length; i++) {
        if (values[i] >= 0 && (min_pos == NO_INTERSECT || values[i] < min_pos)) {
            min_pos = values[i];
        }
    }
    return min_pos;
}


#define EPSILON 1e-4
#define IN_RANGE(a,b)   (((a>(b-EPSILON))&&(a<(b+EPSILON)))?1:0)
/* calculates normal in world space */
Vector calculateNormal(RaycastObject *obj) {
    Point ps_obj = obj->ps_obj;
    Vector normal;
    double x = ps_obj[X];
    double y = ps_obj[Y];
    double z = ps_obj[Z];


    switch(obj->shape->type) {
        case SHAPE_CUBE:
            if (IN_RANGE(x, 0.5)) { /* floating point range check */
                normal = Vector(1, 0, 0);
            } else if (IN_RANGE(x, -0.5)) {
                normal = Vector(-1, 0, 0);
            }
            if (IN_RANGE(y, 0.5)) {
                normal = Vector(0, 1, 0);
            } else if (IN_RANGE(y, -0.5)) {
                normal = Vector(0, -1, 0);
            }
            if (IN_RANGE(z, 0.5)) {
                normal = Vector(0, 0, 1);
            } else if (IN_RANGE(z, -0.5)) {
                normal = Vector(0, 0, -1);
            }
            break;
        case SHAPE_CYLINDER:
            if(IN_RANGE(y, 0.5) || IN_RANGE(y, -0.5)) {
                normal = Vector(0, y, 0);
            } else {
                normal = Vector(x, 0, z);
            }
            break;
        case SHAPE_CONE:
            if(IN_RANGE(y , 0.5)) {
                normal = Vector(0, 1, 0);
            } else if (IN_RANGE(y, -0.5) && IN_RANGE(x, 0) && IN_RANGE(z, 0)) {
                normal = Vector(0, -1, 0);
            } else {
                double theta = atan(ps_obj[Z]/ps_obj[X]);
                double length = 2/sqrt(5) + sqrt(x * x + z * z);
                double x_normal = length * cos(theta);
                double z_normal = length * sin(theta);
                if (x < 0) {
                    x_normal = -1 * x_normal;
                    z_normal = -1 * z_normal;
                }
                Point a(x, y, z);
                Point b(x_normal, y + 1/sqrt(5), z_normal);
                normal = b - a;
            }
            break;
        case SHAPE_SPHERE:
            normal = Vector(x, y, z);
            break;
        default:
            cerr << "Unrecognized shape selected." << endl;
            return Vector();
    }
    normal.normalize();
    normal = transpose(obj->world_to_object) * normal; /* convert to world space */
    normal.normalize();
    return normal;
}
#undef IN_RANGE
#undef EPSILON

#define EPSILON 1e-5
std::vector<RaycastObject> findIntersections(Vector d_world, Point origin_world)
{
    std::vector<RaycastObject> t_objects;
    /* determine intersection with any shape */
    for (int k = 0; k < nodes.size(); k++) {
        Matrix world_to_object = invert(nodes[k].modelView);
        Vector d_obj = world_to_object * d_world;
        Point  p_obj = world_to_object * (origin_world + d_world * EPSILON); 
        for (int l = 0; l < nodes[k].primitives.size(); l++) {
            float t = Intersect(p_obj, d_obj, nodes[k].primitives[l]);
            if (t >= 0) {
                RaycastObject this_object;
                this_object.t = t;
                this_object.shape = nodes[k].primitives[l];
                this_object.ps_obj = p_obj + (t * d_obj);
                this_object.eye_world = camera->GetEyePoint();
                this_object.object_to_world = nodes[k].modelView; 
                this_object.ps_world = nodes[k].modelView * this_object.ps_obj;
                this_object.world_to_object = world_to_object;
                t_objects.push_back(this_object);
            }
        }
    }
    return t_objects;
}
#undef EPSILON

/* calculates intensity of a pixel based on shape and global colors */ 
double calculateIntensity (RaycastObject *obj, int channel, int depth) {
    if (depth < 0) {
        return 0;     
    }
    /* color pixel using normals and lights */
    double I_lambda, k_a, O_alambda, I_mlambda, k_d, O_dlambda;
    Vector L_i, N;
    int nLights = parser->getNumLights();

    /* new variables for recursive raytracer */
    double fatti = 1, k_s, O_slambda, n, O_rlambda, I_rlambda;
    Vector R_i, V;

    SceneGlobalData global_data;
    parser->getGlobalData(global_data);

    /* populate variables */
    k_a = global_data.ka;
    k_d = global_data.kd;
    k_s = global_data.ks;

    O_alambda = obj->shape->material.cAmbient.channels[channel];
    O_dlambda = obj->shape->material.cDiffuse.channels[channel];
    O_slambda = obj->shape->material.cSpecular.channels[channel];
    O_rlambda = obj->shape->material.cReflective.channels[channel];

    N = calculateNormal(obj);
    V = obj->ps_world - obj->eye_world;
    V.normalize();
    n = obj->shape->material.shininess;

    /* calculate the rest of everything based on lights */
    double sum = 0;
    for (int i = 0; i < nLights; i++) {
        SceneLightData light_data;
        parser->getLightData(i, light_data);
        
        L_i = light_data.pos - obj->ps_world;
        L_i.normalize();

        std::vector<RaycastObject> t_objects = findIntersections(L_i, obj->ps_world);
        if (t_objects.empty()) {
            I_mlambda = light_data.color.channels[channel];
            double n_dot_l = dot(N, L_i);
            if (n_dot_l < 0) {
               n_dot_l = 0;
            }
            R_i = dot(N, -L_i) * 2 * N + L_i;
            R_i.normalize();
            double r_dot_v = dot(R_i, V);
            sum += fatti * I_mlambda * (k_d * O_dlambda * n_dot_l + k_s * O_slambda * pow(r_dot_v, n));
        } 
        t_objects.clear();
    }

    Vector next_ray = dot(N, -V) * 2 * N + V;
    std::vector<RaycastObject> t_objects = findIntersections(next_ray, obj->ps_world);
    int len = t_objects.size(); 
    if (len > 0) { /* if there is an intersection */
        RaycastObject first_obj = t_objects[0];
        for (int k = 1; k < len; k++) {
            if (t_objects[k].t < first_obj.t) {
                first_obj = t_objects[k];
            }
        }
        I_rlambda = calculateIntensity(&first_obj, channel, depth - 1);
        t_objects.clear();
        I_lambda = k_a * O_alambda + sum + k_s * O_rlambda * I_rlambda;
    } else {
        I_lambda = k_a * O_alambda + sum;
    }

    if (I_lambda < 0) I_lambda = 0;
    if (I_lambda > 1) I_lambda = 1;
    return I_lambda;
}
