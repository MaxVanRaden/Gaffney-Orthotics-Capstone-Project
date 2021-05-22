#ifndef GAFFNEY_ORTHOTICS_CAPSTONE_PROJECT_MESHEDITOR_H
#define GAFFNEY_ORTHOTICS_CAPSTONE_PROJECT_MESHEDITOR_H

#include "Entity.h"
#include "backend/src/engine/maths.h"
#include "backend/src/engine/texture.h"
#include "backend/src/engine/shaders.h"
#include "backend/src/engine/render.h"

enum EditorState {
    STATE_SELECT_ENTITY,
    STATE_SELECT_VERTICES
};

enum Axis {
    X,
    Y,
    Z
};

class MeshEditor {
public:
    MeshEditor();
    ~MeshEditor();

    void draw();
    void run(int width, int height);
    void add_model(const char* str, int fileformat);
    char* export_model(const char* fileformat);
    void set_camera(float zoom, float x, float y, float z, float yaw, float pitch, float roll);
    void scale_all_entities(float factor);
    void on_mouse_up(int x, int y, int x2, int y2);
    uint32_t get_export_strlen() const;
    void set_undo();
    void undo_model();
    void redo_model();
    void flip_axis();

    bool is_mouse_over_arrow(vec3 o, vec3 d, mat4 transform);

private:
    void translate_vertices_along_axis(Axis axis);
    vec3 calculate_avg_pos_selected_vertices();

    std::vector<Entity> entities;
    int selectedEntity;
    std::vector<Entity> undostack;
    std::vector<Entity> redostack;

    bool axis_clicked;
    bool fliparrows;

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
    Model arrow;

    EditorState state;
};

#endif //GAFFNEY_ORTHOTICS_CAPSTONE_PROJECT_MESHEDITOR_H
