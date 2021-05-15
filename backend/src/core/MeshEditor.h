#ifndef GAFFNEY_ORTHOTICS_CAPSTONE_PROJECT_MESHEDITOR_H
#define GAFFNEY_ORTHOTICS_CAPSTONE_PROJECT_MESHEDITOR_H

#include "Entity.h"
#include "backend/src/engine/maths.h"
#include "backend/src/engine/texture.h"
#include "backend/src/engine/shaders.h"
#include "backend/src/engine/render.h"
#include <stack>;

class MeshEditor {
public:
    MeshEditor();
    ~MeshEditor();

    void run(int width, int height);
    void add_model(const char* str, int fileformat);
    char* export_model(const char* fileformat);
    void set_camera(float zoom, float x, float y, float z, float yaw, float pitch, float roll);
    void scale_all_entities(float factor);
    void on_mouse_up(int x, int y, int x2, int y2);
    uint32_t get_export_strlen() const;
    void translate_vertex();
    void undo_model();

private:
    std::vector<Entity> entities;
    std::stack<Model> undostack;

    Mesh billboard;
    Texture circle;
    Rect viewport;

    mat4 projection;
    PickingShader pshader{};
    StaticShader shader{};
    BillboardShader bshader{};
    Framebuffer pickbuffer;
    Camera camera{};
    Model stairs;
    Model cylinderModel;
    uint32_t export_strlen;
};

#endif //GAFFNEY_ORTHOTICS_CAPSTONE_PROJECT_MESHEDITOR_H
