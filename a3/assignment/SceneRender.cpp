#include "SceneRender.h"
#include "SceneData.h"
#include <GL/glui.h>
#include <iostream>
enum dimensions {X, Y, Z};

Matrix buildTransformation(SceneTransformation *trans);
Matrix CalcRotate(Vector axis, double gamma);
void renderShape (int shapeType);

SceneRender::SceneRender(Cube *cube, Cylinder *cylinder, Cone *cone, Sphere *sphere, int *segmentsX, int *segmentsY)
{
    this->cube = cube;
    this->cone = cone;
    this->sphere = sphere;
    this->cylinder = cylinder;
    this->segmentsX = segmentsX;
    this->segmentsY = segmentsY;
}

void SceneRender::flatten(SceneNode *root, Matrix transformation) 
{
    RenderNode node;
    for (int i = 0; i < root->transformations.size(); i++) {
        transformation = transformation * buildTransformation(root->transformations[i]);
    }
    node.composite = transformation;
    node.primitives = root->primitives;

    nodes.push_back(node);

    for (int i = 0; i < root->children.size(); i++) {
        flatten(root->children[i], transformation);
    }
}

void SceneRender::render() 
{
    for (int i = 0; i < nodes.size(); ++i)
    {
       /* glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glLoadMatrixd(nodes[i].composite.unpack());*/
        for (int j = 0; j < nodes[i].primitives.size(); ++j) {
            ScenePrimitive *obj = nodes[i].primitives[j];
            SceneColor spec = obj->material.cSpecular, amb = obj->material.cAmbient, diff = obj->material.cDiffuse;
        
            std::cerr << "specular: " << spec.r << " " << spec.g << " " << spec.b << " " << spec.a << endl;
            std::cerr << "ambient: " << amb.r << " " << amb.g << " " << amb.b << " " << amb.a << endl;
            std::cerr << "diffuse: " << diff.r << " " << diff.g << " " << diff.b << " " << diff.a << endl;

            // glEnable (GL_COLOR_MATERIAL);
           // glEnable(GL_POLYGON_OFFSET_FILL);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, obj->material.cAmbient.channels);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, obj->material.cDiffuse.channels);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, obj->material.cSpecular.channels);
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, obj->material.cEmissive.channels);
            // FIND A WAY TO DO REFLECTIVE AND TRANSPARENT
            // glMaterialfv(GL_FRONT_AND_BACK, GL_REFLECTIVE, obj->material.cReflective.channels);
            // glMaterialfv(GL_FRONT_AND_BACK, GL_TRANSPARENT, obj->material.cTransparent.channels);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, obj->material.shininess); 
 
            Shape *shape;
            switch (obj->type) {
                case SHAPE_CUBE:
                    std::cerr << "Making a cube" << std::endl; 
                    shape = cube;
                    break;
                case SHAPE_CYLINDER:
                    std::cerr << "Making a cylinder" << std::endl; 
                    shape = cylinder;
                    break;
                case SHAPE_CONE:
                    std::cerr << "Making a cone" << std::endl; 
                    shape = cone;
                    break;
                case SHAPE_SPHERE:
                    std::cerr  << "Making a sphere" << std::endl; 
                    shape = sphere;
                    break;
                default:
                    shape = cube;
            }
            //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            shape->setSegments(*segmentsX, *segmentsY);
            shape->draw();
        } 
    } 
}

Matrix buildTransformation(SceneTransformation *trans)
{
    Matrix return_matrix;
    switch (trans->type) {
        case TRANSFORMATION_TRANSLATE:
            return_matrix = 
                Matrix(1, 0, 0, trans->translate[X],
                       0, 1, 0, trans->translate[Y],
                       0, 0, 1, trans->translate[Z],
                       0, 0, 0, 1);
            break;
        case TRANSFORMATION_ROTATE:
            return_matrix = CalcRotate(trans->rotate, trans->angle);
            break;
        case TRANSFORMATION_SCALE:
            return_matrix =
                Matrix(trans->scale[X], 0, 0, 0,
                       0, trans->scale[Y], 0, 0,
                       0, 0, trans->scale[Z], 0,
                       0, 0, 0, 1);
            break;
        case TRANSFORMATION_MATRIX:
            return_matrix = trans->matrix;
            break;
    }
    return return_matrix;
}

/* rotations around origin */
Matrix CalcRotate(Vector axis, double gamma) {
    double theta = atan2(axis[Z], axis[X]);
    double phi = -atan2(axis[Y], sqrt(axis[X] * axis[X] + axis[Z] * axis[Z]));
    return rotX_mat(gamma) * rotY_mat(theta) * rotZ_mat(phi);
}
