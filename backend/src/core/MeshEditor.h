#ifndef GAFFNEY_ORTHOTICS_CAPSTONE_PROJECT_MESHEDITOR_H
#define GAFFNEY_ORTHOTICS_CAPSTONE_PROJECT_MESHEDITOR_H

#include "Entity.h"
#include "backend/src/engine/maths.h"
#include "backend/src/engine/texture.h"
#include "backend/src/engine/shaders.h"
#include "backend/src/engine/render.h"

class MeshEditor {
public:
    MeshEditor();
    ~MeshEditor();

    void run();
    void add_model(const char* str);
    char* export_model(int ID, const char* fileformat);
    void set_camera(float zoom, float x, float y, float z, float yaw, float pitch, float roll);
    void scale_all_entities(float factor);
    char* export_new(int ID, const char* fileformat);
    uint32_t get_export_strlen();

private:
    std::vector<Entity> entities;

    uint32_t export_strlen;
    StaticShader shader{};
    Camera camera{};
    Model stairs;
};

#endif //GAFFNEY_ORTHOTICS_CAPSTONE_PROJECT_MESHEDITOR_H
