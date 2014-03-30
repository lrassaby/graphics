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
double minPositive(double *values, int length);
double quadraticIntersect(double A, double B, double C);
double sq(double a) { return a * a; }
bool isOutOfBounds(double min, double max, double tocheck) {
    return (tocheck < min || tocheck > max);
};


/******************************************************/

void setupCamera();
void updateCamera();

void setPixel(GLubyte* buf, int x, int y, int r, int g, int b) {
	buf[(y*pixelWidth + x) * 3 + 0] = (GLubyte)r;
	buf[(y*pixelWidth + x) * 3 + 1] = (GLubyte)g;
	buf[(y*pixelWidth + x) * 3 + 2] = (GLubyte)b;
}

struct RaycastObject { /* a point on a shape */
    double t;
    ScenePrimitive *shape;
    Point p_obj;
    Vector d_obj;
    Matrix obj_to_world;
};

enum colors {R, G, B};

/* calculates intensity of a pixel based on shape and global colors */ 
double calculateIntensity (RaycastObject *obj, int channel) {
    /* color pixel using normals and lights */
    double I_lambda, k_a, O_alambda, m, I_mlambda, k_d, O_dlambda;
    Vector L_m, N;
    int nLights;

    /* for all lights */

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
            std::vector<RaycastObject> t_objects;
            Vector d = generateRay(i, j, camera);
            Vector d_world = camera->GetInverseTransformMatrix() * d;
            d_world.normalize();
            for (int k = 0; k < nodes.size(); k++) {
                Matrix inv_mv = invert(nodes[k].modelView);
                Vector d_obj = inv_mv * d_world;
                Point  p_obj = inv_mv * camera->GetEyePoint(); 
                for (int l = 0; l < nodes[k].primitives.size(); l++) {
                    float t = Intersect(p_obj, d_obj, nodes[k].primitives[l]);
                    if (t >= 0) {
                        if (isectOnly) {
                            setPixel(pixels, i, j, 255, 255, 255);
                        } else { /* coloring, etc. */
                            RaycastObject this_object;
                            this_object.t = t;
                            this_object.shape = nodes[k].primitives[l];
                            this_object.d_obj = d_obj;
                            this_object.p_obj = p_obj;
                            this_object.obj_to_world = inv_mv;
                            t_objects.push_back(this_object);
                        }
                    }
                }
            }
            if (!isectOnly) {
                int len = t_objects.size();
                if (len > 0) { /* if there is an intersection */
                    RaycastObject first_obj = t_objects[0];
                    for (int k = 1; k < len; k++) {
                        if (t_objects[k].t < first_obj.t) {
                            first_obj = t_objects[k];
                        }
                    }
                } /* else no intersect */
                t_objects.clear();
            }
            setPixel(calculateIntensity(&first_obj, R), 
                     calculateIntensity(&first_obj, G);
                     calculateIntensity(&first_obj, B));
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
    Point p(i, j, -1);
    Point eyePoint(0, 0, 0);

    /* convert p to normalized film space, p_film */
    Point p_film = convertToNormalizedFilm(p);

    d = (p_film  - eyePoint);
    return d; 
}

Point convertToNormalizedFilm(Point p) 
{
    return Point ((2.0f * p[X] / screenWidth) - 1.0f, 
            (2.0f * p[Y] / screenWidth) - 1.0f, -1.0);
    /* according to slides???
    return Point ((2.0f * p[X] / screenWidth) - 1.0f, 
                   1.0f - (2.0f * p[Y] / screenHeight), -1.0);
    */
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
    int num_faces = 6;
    double components[num_faces];
    Point intersect; 
    if (rayV[X] != 0) {
        /* x == 0.5 plane */
        components[0] = (0.5 - eyePointP[X]) / rayV[X];
        intersect = eyePointP + components[0] * rayV; 
        if (isOutOfBounds(-0.5, 0.5, intersect[Y]) || isOutOfBounds(-0.5, 0.5, intersect[Z])) {
            components[0] = NO_INTERSECT;
        }

        /* x == -0.5 plane */
        components[1] = (-0.5 - eyePointP[X]) / rayV[X];
        intersect = eyePointP + components[1] * rayV; 
        if (isOutOfBounds(-0.5, 0.5, intersect[Y]) || isOutOfBounds(-0.5, 0.5, intersect[Z])) {
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
        if (isOutOfBounds(-0.5, 0.5, intersect[X]) || isOutOfBounds(-0.5, 0.5, intersect[Z])) {
            components[2] = NO_INTERSECT;
        }

        /* y == -0.5 plane */
        components[3] = (-0.5 - eyePointP[Y]) / rayV[Y];
        intersect = eyePointP + components[3] * rayV; 
        if (isOutOfBounds(-0.5, 0.5, intersect[X]) || isOutOfBounds(-0.5, 0.5, intersect[Z])) {
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
        if (isOutOfBounds(-0.5, 0.5, intersect[X]) || isOutOfBounds(-0.5, 0.5, intersect[Y])) {
            components[4] = NO_INTERSECT;
        }

        /* z == -0.5 plane */
        components[5] = (-0.5 - eyePointP[Z]) / rayV[Z];
        intersect = eyePointP + components[5] * rayV; 
        if (isOutOfBounds(-0.5, 0.5, intersect[X]) || isOutOfBounds(-0.5, 0.5, intersect[Y])) {
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
double IntersectCylinder(Point eyePointP, Vector rayV) 
{
    Point intersect; 
    double components[3];
    double A, B, C;
    Point objCenter(0, 0, 0);
    Vector eyeVector = eyePointP - objCenter;

    A = sq(rayV[X]) + sq(rayV[Z]);
    B = 2 * (eyePointP[X] * rayV[X] + eyePointP[Z] * rayV[Z]);
    C = sq(eyePointP[X]) + sq(eyePointP[Z]) - .25;

    components[T_BODY] = quadraticIntersect(A, B, C);
    intersect = eyePointP + components[T_BODY] * rayV; 
    if (isOutOfBounds(-0.5, 0.5, intersect[Y])) {
        components[T_BODY] = NO_INTERSECT;
    }

    if (rayV[Y] != 0) {
        components[T_TOP] = (0.5 - eyePointP[Y]) / rayV[Y];
        intersect = eyePointP + components[T_TOP] * rayV; 
        if (isOutOfBounds(0, 0.25, sq(intersect[X]) + sq(intersect[Z]))) {
            components[T_TOP] = NO_INTERSECT;
        }
        components[T_BOTTOM] = (-0.5 - eyePointP[Y]) / rayV[Y];
        intersect = eyePointP + components[T_BOTTOM] * rayV; 
        if (isOutOfBounds(0, 0.25, sq(intersect[X]) + sq(intersect[Z]))) {
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
double IntersectCone(Point eyePointP, Vector rayV)
{
    Point intersect; 
    double components[2];
    double A, B, C;
    Point objCenter(0, 0, 0);
    Vector eyeVector = eyePointP - objCenter;

    A = sq(rayV[X]) + sq(rayV[Z]) - 0.25 * sq(rayV[Y]);
    B = (2.0 * eyePointP[X] * rayV[X]) + 
        (2.0 * eyePointP[Z] * rayV[Z]) - 
        (0.5 * eyePointP[Y] * rayV[Y]) + 
        0.25 * rayV[Y];
    C = sq(eyePointP[X]) + sq(eyePointP[Z]) - 
        0.25 * sq(eyePointP[Y]) + 0.25 * eyePointP[Y] - 0.0625;

    components[T_BODY] = quadraticIntersect(A, B, C);
    intersect = eyePointP + components[T_BODY] * rayV; 
    if (isOutOfBounds(-0.5, 0.5, intersect[Y])) {
        components[T_BODY] = NO_INTERSECT;
    }
    if (rayV[Y] != 0) {
        components[T_BASE] = (-0.5 - eyePointP[Y]) / rayV[Y];
        intersect = eyePointP + components[T_BASE] * rayV; 
        if (isOutOfBounds(0, 0.25, sq(intersect[X]) + sq(intersect[Z]))) {
            components[T_BASE] = NO_INTERSECT;
        }
    } else {
        components[T_BASE] = NO_INTERSECT;
    }
    return minPositive(components, 2);
}
#undef T_BODY
#undef T_BASE

/* both arguments in object space */
double IntersectSphere(Point eyePointP, Vector rayV) {
    double A, B, C; 
    Point objCenter(0, 0, 0);
    Vector eyeVector = eyePointP - objCenter;

    A = dot(rayV, rayV);
    B = 2 * dot(eyeVector, rayV);
    C = dot(eyeVector, eyeVector) - 0.25; 

    return quadraticIntersect(A, B, C);

}

double quadraticIntersect(double A, double B, double C) 
{
    float determinant = B * B - 4.0f * A * C; 
    if (determinant < 0) {
        //cerr << "No intersect" << endl;
        return NO_INTERSECT;
    } else if (determinant == 0) {
        //cerr << "One intersection at " << -1 * B/ (2.0f * A) << endl;
        return -1 * B / (2.0f * A);
    } else {
        double solutions[2];
        solutions[0] = (-1 * B - sqrt(determinant)) / (2.0f * A);
        solutions[1] = (-1 * B + sqrt(determinant)) / (2.0f * A);
/*
        cerr << "Two intersections at " << (-1 * B + sqrt(determinant)) / (2.0f * A)
                  << " and "            << (-1 * B - sqrt(determinant)) / (2.0f * A) << endl;
                  */
        return minPositive(solutions, 2);
    }
}

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
