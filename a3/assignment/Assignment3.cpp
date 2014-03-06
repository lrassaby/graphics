#define NUM_OPENGL_LIGHTS 8

//#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
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
int  wireframe = 0;
int  fillObj = 1;
int  segmentsX = 20;
int  segmentsY = 20;

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
// string filenamePath = "/Users/louisrassaby/Dropbox/Comp175/part2/a3/assignment/data/general/ball.xml";
//string filenamePath = "/Users/louisrassaby/Dropbox/Comp175/part2/a3/assignment/data/general/ball2.xml";
string filenamePath = "/Users/jwoogerd/Desktop/a3-Mac/data/general/cone.xml";
GLUI_EditText* filenameTextField = NULL;


/** these are the global variables used for rendering **/
Cube* cube = new Cube();
Cylinder* cylinder = new Cylinder();
Cone* cone = new Cone();
Sphere* sphere = new Sphere();
Shape* shape = NULL;
SceneParser* parser = NULL;
Camera* camera = new Camera();

/******************************************************/
/* global variables added by Jayme and Louis */
enum dimensions {X, Y, Z};

struct RenderNode {
	Matrix modelView;
	Matrix projection;
    std::vector<ScenePrimitive*> primitives;
};

std::vector<RenderNode> nodes;
bool initialload = true;
Matrix calcRotate(Vector axis, double gamma);
void flatten(SceneNode *root, Matrix modelView, Matrix projection);
void render();
/* end globals */
/******************************************************/

void setupCamera();

void callback_load(int id) {
	char curDirName [2048];
	if (filenameTextField == NULL) {
		return;
	}
	printf ("%s\n", filenameTextField->get_text());

    nodes.clear();
    initialload = true;

	if (parser != NULL) {
		delete parser;
	}
	parser = new SceneParser (filenamePath);
	cout << "success? " << parser->parse() << endl;

	setupCamera();
}

void renderShape (int shapeType) {
	switch (shapeType) {
	case SHAPE_CUBE:
		shape = cube;
		break;
	case SHAPE_CYLINDER:
		shape = cylinder;
		break;
	case SHAPE_CONE:
		shape = cone;
		break;
	case SHAPE_SPHERE:
		shape = sphere;
		break;
	case SHAPE_SPECIAL1:
		shape = cube;
		break;
	default:
		shape = cube;
	}
	shape->setSegments(segmentsX, segmentsY);
	shape->draw();
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

	glutPostRedisplay();
}


/***************************************** setupCamera() *****************/
void setupCamera()
{
	SceneCameraData cameraData;
	parser->getCameraData(cameraData);

	camera->Reset();  //note that this is a new function. Be sure to set values for near and far plane!
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



/***************************************** setLight() *****************/
void setLight(const SceneLightData &light)
{
    // Zero ambient light because the ambient light for the scene is
    // set with GL_LIGHT_MODEL_AMBIENT
    float zero[] = { 0, 0, 0, 0 };

    // There are 8 lights in OpenGL, GL_LIGHT0 to GL_LIGHT7, and
    // each GL_LIGHT* is one greater than the previous one.  This
    // means the nth light is GL_LIGHT0 + n.
    int id = GL_LIGHT0 + light.id;

    glLightfv(id, GL_AMBIENT, zero);
    glLightfv(id, GL_DIFFUSE, &light.color.r);
    glLightfv(id, GL_SPECULAR, &light.color.r);
    glLightf(id, GL_CONSTANT_ATTENUATION, light.function[0]);
    glLightf(id, GL_LINEAR_ATTENUATION, light.function[1]);
    glLightf(id, GL_QUADRATIC_ATTENUATION, light.function[2]);

    if (glIsEnabled(id))
        cout << "warning: GL_LIGHT" << light.id << " enabled more than once, not supposed to happen" << endl;

    switch (light.type)
    {
        case LIGHT_POINT:
        {
            // Convert from double[] to float[] and make sure the w coordinate is correct 
            float position[] = { light.pos[0], light.pos[1], light.pos[2], 1 };
            glLightfv(id, GL_POSITION, position);
			glEnable(id);
            break;
        }

        case LIGHT_DIRECTIONAL:
        {
            // Convert from double[] to float[] and make sure the direction vector is normalized (it isn't for a lot of scene files)
            Vector direction = -light.dir;
			direction.normalize();
            float position[] = { direction[0], direction[1], direction[2], direction[3] };
            glLightfv(id, GL_POSITION, position);
			glEnable(id);
            break;
        }
    }
}


/***************************************** applyMaterial() *****************/
void applyMaterial(const SceneMaterial &material)
{
	SceneGlobalData globalData;
	parser->getGlobalData(globalData);

	SceneMaterial material_local = material;
	material_local.cAmbient.r *= globalData.ka;
	material_local.cAmbient.g *= globalData.ka;
	material_local.cAmbient.b *= globalData.ka;
	material_local.cDiffuse.r *= globalData.kd;
	material_local.cDiffuse.g *= globalData.kd;
	material_local.cDiffuse.b *= globalData.kd;
	material_local.cSpecular.r *= globalData.ks;
	material_local.cSpecular.g *= globalData.ks;
	material_local.cSpecular.b *= globalData.ks;
	material_local.cReflective.r *= globalData.ks;
	material_local.cReflective.g *= globalData.ks;
	material_local.cReflective.b *= globalData.ks;
	material_local.cTransparent.r *= globalData.kt;
	material_local.cTransparent.g *= globalData.kt;
	material_local.cTransparent.b *= globalData.kt;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &material_local.cAmbient.r);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &material_local.cDiffuse.r);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &material_local.cSpecular.r);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &material_local.cEmissive.r);
}


/***************************************** myGlutDisplay() *****************/

void myGlutDisplay(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (parser == NULL) {
		return;
	}

	camera->SetViewAngle(viewAngle);
	glMatrixMode(GL_PROJECTION);
	Matrix projection = camera->GetProjectionMatrix();
	glLoadMatrixd(projection.unpack());
	Point eye(eyeX, eyeY, eyeZ);
	Vector look(lookX, lookY, lookZ);
	Vector up(0, 1, 0);
	camera->Orient(eye, look, up);
	camera->RotateV(camRotV);
	camera->RotateU(camRotU);
	camera->RotateW(camRotW);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(camera->GetModelViewMatrix().unpack());

	//disable all the lights, and then enable each one...
	for (int i = 0; i < NUM_OPENGL_LIGHTS; i++) {
		glDisable(GL_LIGHT0 + i);
	}

	if (initialload) {
		SceneNode *root = parser->getRootNode();
		flatten(root, Matrix(), Matrix());
		initialload = false;
	}

	//drawing the axes
	glEnable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0, 0, 0); glVertex3f(1.0, 0, 0);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, 0, 0); glVertex3f(0.0, 1.0, 0);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, 0); glVertex3f(0, 0, 1.0);
	glEnd();
	glColor3f(1.0, 0.0, 0.0);


	if (wireframe) {
		glDisable(GL_POLYGON_OFFSET_FILL);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//TODO: draw wireframe of the scene...
		render();
		// note that you don't need to applyMaterial, just draw the geometry
	}

    glDisable(GL_COLOR_MATERIAL);
	int numLights = parser->getNumLights();
	for (int i = 0; i < numLights; i++) {
		SceneLightData lightData;
		parser->getLightData(i, lightData);
		setLight(lightData);
	}

	glEnable(GL_LIGHTING);
	if (fillObj == 1) {
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//TODO: render the scene...
		render();
		// note that you should always applyMaterial first, then draw the geometry
	}
	glDisable(GL_LIGHTING);
	
	camera->RotateV(-camRotV);
	camera->RotateU(-camRotU);
	camera->RotateW(-camRotW);

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
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(500, 500);

	main_window = glutCreateWindow("COMP 175 Assignment 3");
	glutDisplayFunc(myGlutDisplay);
	glutReshapeFunc(myGlutReshape);

	glShadeModel (GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);

	// Specular reflections will be off without this, since OpenGL calculates
	// specular highlights using an infinitely far away camera by default, not
	// the actual location of the camera
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	// Show all ambient light for the entire scene (not one by default)
	GLfloat one[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, one);

	glPolygonOffset(1, 1);



	/****************************************/
	/*         Here's the GLUI code         */
	/****************************************/

	GLUI* glui = GLUI_Master.create_glui("GLUI");

	filenameTextField = new GLUI_EditText( glui, "Filename:", filenamePath);
	filenameTextField->set_w(300);
	glui->add_button("Load", 0, callback_load);
	
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

	glui->add_column(true);

	GLUI_Panel *render_panel = glui->add_panel("Render");
	new GLUI_Checkbox(render_panel, "Wireframe", &wireframe);
	new GLUI_Checkbox(render_panel, "Fill", &fillObj);
	(new GLUI_Spinner(render_panel, "Segments X:", &segmentsX))
		->set_int_limits(3, 60);
	(new GLUI_Spinner(render_panel, "Segments Y:", &segmentsY))
		->set_int_limits(3, 60);


	glui->add_button("Quit", 0, (GLUI_Update_CB)exit);

	glui->set_main_gfx_window(main_window);

	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(myGlutIdle);

	glutMainLoop();

	return EXIT_SUCCESS;
}


/***********************************************************************************************/
/***************************** Code added by Louis and Jayme ***********************************/
/***********************************************************************************************/


void flatten(SceneNode *root, Matrix modelView, Matrix projection) 
{
    RenderNode node;
    Matrix transmat;
    SceneTransformation *trans;
    for (int i = 0; i < root->transformations.size(); i++) {
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
	    		projection = projection * transmat;
    			break;
    		case TRANSFORMATION_MATRIX:
    			transmat = trans->matrix;
    			projection = projection * transmat;
    			break;
    	}
    }
    node.primitives = root->primitives;
    
    node.modelView = modelView;
    node.projection = projection;

    nodes.push_back(node);

    for (int i = 0; i < root->children.size(); i++) {
        flatten(root->children[i], modelView, projection);
    }
}

void render() 
{
    for (int i = 0; i < nodes.size(); ++i) {
        for (int j = 0; j < nodes[i].primitives.size(); ++j) {
            ScenePrimitive *obj = nodes[i].primitives[j];
            if (!wireframe) {
            	applyMaterial(obj->material);
            }
            
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glMultMatrixd(camera->GetProjectionMatrix().unpack());
            glMultMatrixd(nodes[i].projection.unpack());

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glMultMatrixd(camera->GetModelViewMatrix().unpack());
            glMultMatrixd(nodes[i].modelView.unpack());
            
            renderShape(obj->type);
        } 
    } 
}

/* rotations around origin */
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
    
    //return rotX_mat(gamma) * rotZ_mat(phi) * rotY_mat(theta);
}
