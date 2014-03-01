#ifndef SCENE_RENDER_H 
#define SCENE_RENDER_H 

#include <vector>
#include "SceneData.h"
#include "Shape.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Sphere.h"

struct RenderNode {
    Matrix composite;
    std::vector<ScenePrimitive*> primitives;
};

class SceneRender {
    public:
        void flatten(SceneNode *root, Matrix transformation);
        void render();
        SceneRender(){};
        SceneRender(Cube *cube, Cylinder *cylinder, Cone *cone, Sphere *sphere, int *segmentsX, int *segmentsY);
    private:
        std::vector<RenderNode> nodes;
        Cube* cube;
        Cylinder* cylinder;
        Cone* cone;
        Sphere* sphere;
        int *segmentsX, *segmentsY;
};

#endif
