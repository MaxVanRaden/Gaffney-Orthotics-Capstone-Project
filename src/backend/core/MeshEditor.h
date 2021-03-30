#ifndef GAFFNEY_ORTHOTICS_CAPSTONE_PROJECT_MESHEDITOR_H
#define GAFFNEY_ORTHOTICS_CAPSTONE_PROJECT_MESHEDITOR_H

#include "Entity.h"
#include "src/backend/engine/maths.h"
#include "src/backend/engine/texture.h"
#include "src/backend/engine/shaders.h"
#include "src/backend/engine/render.h"

class MeshEditor {
public:
    MeshEditor();
    ~MeshEditor();

    void run();

private:
    std::vector<Entity> entities;

    StaticShader shader{};
    Camera camera{};
    Model stairs;
};

#endif //GAFFNEY_ORTHOTICS_CAPSTONE_PROJECT_MESHEDITOR_H
