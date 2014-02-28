#include "Render.h"

void Render::flatten(SceneNode *root) 
{
    for (int i = 0; i < root->children.size(); i++) {
        flatten(root->children[i]);
    }
}

void Render::render() { }
