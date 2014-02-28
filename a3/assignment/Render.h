#ifndef RENDER_H 
#define RENDER_H 

#include <vector>
#include "SceneData.h"

struct RenderNode {
    Matrix composite;
    std::vector<ScenePrimitive*> primitives;
};

class Render {
    public:
        void flatten(SceneNode *root, Matrix transformation);
        void render();
    private:
        std::vector<RenderNode> nodes;
};

#endif
