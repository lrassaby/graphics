#include "Render.h"
#include "SceneData.h"

enum dimensions {X, Y, Z};

void Render::flatten(SceneNode *root, Matrix transformation) 
{
    RenderNode node;
    for (int i = 0; i < root->transformations.size(); i++) {
        //transformation = root->transformations[i];

    }

    //nodes.push_back();

    for (int i = 0; i < root->children.size(); i++) {
        flatten(root->children[i], transformation);
    }
}

void Render::render() {}

/* private */

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
            /* TODO: This. */
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
