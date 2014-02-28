#include "Render.h"
#include "SceneData.h"

enum dimensions {X, Y, Z};

Matrix buildTransformation(SceneTransformation *trans);
Matrix CalcRotate(Vector axis, double gamma);

void Render::flatten(SceneNode *root, Matrix transformation) 
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

void Render::render() 
{
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

