#ifndef RENDER_H 
#define RENDER_H 

#include <vector>
#include "SceneData.h"

struct RenderNode {
    Matrix composite;
    SceneColor color;
    std::vector<ScenePrimitive*> objects;
};

class Render {
    public:
        void flatten(SceneNode *root);
        void render();
    private:
        std::vector<struct RenderNode> nodes;
};

#endif
