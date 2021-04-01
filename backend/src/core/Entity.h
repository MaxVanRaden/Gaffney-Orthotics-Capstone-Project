#ifndef GAFFNEY_ORTHOTICS_CAPSTONE_PROJECT_ENTITY_H
#define GAFFNEY_ORTHOTICS_CAPSTONE_PROJECT_ENTITY_H

#include "backend/src/engine/maths.h"
#include "backend/src/engine/texture.h"
#include "backend/src/engine/shaders.h"
#include "backend/src/engine/render.h"

#define MAX_REVERT_COUNT 50

class Entity {
public:
    Entity();
    explicit Entity(std::string file);
    ~Entity();

    void draw(StaticShader& shader);
    void set_position(vec3 pos);
    void set_rotation(vec3 rotate);
    void set_scale(vec3 scale);

private:
    Model current; //current model
    Model start; //the model before any changes were made
    Model previous[MAX_REVERT_COUNT]; //an array of the last MAX_REVERT_COUNT number of changes (for undoing)
};

#endif //GAFFNEY_ORTHOTICS_CAPSTONE_PROJECT_ENTITY_H
