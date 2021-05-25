#ifndef GAFFNEY_ORTHOTICS_CAPSTONE_PROJECT_MESHEDITOR_H
#define GAFFNEY_ORTHOTICS_CAPSTONE_PROJECT_MESHEDITOR_H

#include "Entity.h"
#include "backend/src/engine/maths.h"
#include "backend/src/engine/texture.h"
#include "backend/src/engine/shaders.h"
#include "backend/src/engine/render.h"

#define INVALID_CROSS_SECTION 0xFFFFFF

enum EditorState {
    STATE_SELECT_ENTITY,
    STATE_SELECT_VERTICES,
    STATE_SELECT_CROSS_SECTION
};

class MeshEditor {
public:
    MeshEditor();
    ~MeshEditor();

    void draw();
    void run(int width, int height);
    void camera_controls();

    void add_model(const char* str, int fileformat);
    char* export_model(const char* fileformat);
    void set_camera(float zoom, float posX, float posY, float posZ, float lookAtX, float lookAtY, float lookAtZ);
    void scale_all_entities(float factor);
    void on_mouse_up(int x, int y, int x2, int y2);
    uint32_t get_export_strlen() const;
    void translate_vertex();

    bool is_mouse_over_arrow(vec3 o, vec3 d, mat4 transform);

private:
    std::vector<Entity> entities;
    int selectedEntity;
    bool mouseDown;
    bool waitForRelease;
    bool shiftDown;
    bool zoomOut;
    bool zoomIn;
    bool showOverlay;
    vec3 dragDirection;

    bool placedFirstSection;
    float crossSectionBot;
    float crossSectionTop;
    int lastMouseX;
    int lastMouseY;

    Mesh billboard;
    Texture circle;
    Rect viewport;

    mat4 projection;
    PickingShader pshader{};
    StaticShader shader{};
    BillboardShader bshader{};
    Framebuffer pickbuffer;
    //Camera camera{};
    vec3 cameraPos;
    vec3 cameraCenter;
    Model stairs;
    Model cylinderModel;
    uint32_t export_strlen;
    Model arrow;

    EditorState state;
};

#endif //GAFFNEY_ORTHOTICS_CAPSTONE_PROJECT_MESHEDITOR_H
