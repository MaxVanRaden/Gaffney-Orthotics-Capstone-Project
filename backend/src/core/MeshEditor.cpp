#include "MeshEditor.h"
#include "StairsString.h"
#include "CylinderString.h"
#include "ArrowString.h"
#include "assimp/Exporter.hpp"

MeshEditor::MeshEditor() {
    shader.load();
    bshader.load();
    pshader.load();
    camera = {0};
    entities.emplace_back();
    entities.back().load(staircaseobjhardcoded);
    entities.back().set_position( {0, 0, 0} );
    //entities.emplace_back();
    //entities.back().load(staircaseobjhardcoded);
    //entities.back().set_position( {2, 2, 2} );
    projection = perspective_projection(90, 16.0f / 9.0f, 0.01f, 3000.0f);
    move_cam_backwards(&camera, 5);
    camera.x = camera.y = camera.z = 4;
    state = STATE_SELECT_ENTITY;

    billboard = create_billboard();

    //Create a circle texture to be used to show where vertices are on the mesh
    auto* pixels = new unsigned char[64*64*4];
    vec2 center = {32.0f, 32.0f};
    for(int x = 0; x < 64; ++x) {
        for(int y = 0; y < 64; ++y) {
            vec2 pos = {(float)x, (float)y};
            int index = (x+y*64)*4;

            vec2 diff = center - pos;
            f32 distance = length(diff);

            if(distance < 32) {
                pixels[index+0] = 255;
                pixels[index+1] = 255;
                pixels[index+2] = 255;
                pixels[index+3] = 255;
            } else {
                pixels[index+0] = 0;
                pixels[index+1] = 0;
                pixels[index+2] = 0;
                pixels[index+3] = 0;
            }
        }
    }
    circle = load_texture(pixels, 64, 64, GL_LINEAR);
    delete[] pixels;

    cylinderModel = load_model_string(cylinderHardcoded);
    arrow = load_model_string(arrowHardcoded);

    //pickbuffer = create_color_buffer(1920, 1080, GL_LINEAR);
}

void MeshEditor::run(int width, int height) {
    viewport = {0, 0, (float)width, (float)height};

    if(state == STATE_SELECT_ENTITY) {
        int button = glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT);
        if(button == GLFW_PRESS) {
            selectedEntity = -1;
            vec2 mouse;
            int x;
            int y;
            glfwGetMousePos(&x, &y);
            mouse.x = x;
            mouse.y = y;

            vec3 rayposition = {camera.x, camera.y, camera.z};
            vec3 raydirection = raycast(projection, camera, mouse, viewport);

            for (int i = 0; i < entities.size(); ++i) {
                if(entities[i].is_mouse_over(rayposition, raydirection)) {
                    selectedEntity = i;
                    state = STATE_SELECT_VERTICES;
                }
            }
        }
    } else if(state == STATE_SELECT_VERTICES) {
        int keystate = glfwGetKey(GLFW_KEY_TAB);
        if(keystate == GLFW_PRESS) {
            state = STATE_SELECT_ENTITY;
        }
    }

    draw();
}

void MeshEditor::draw() {
    mat4 view = create_view_matrix(camera);
    //view = look_at({camera.x, camera.y, camera.z}, {0, 0, 0});

    shader.bind();
    shader.set_light_pos(14, 14, 14);
    shader.set_light_color(147.0f/255.0f, 108.0f/255.0f,95.0f/255.0f);
    shader.set_camera_pos(camera.x, camera.y, camera.z);
    shader.set_view(view);

    for(Entity& e : entities) {
        e.draw(shader);
    }

    if(state == STATE_SELECT_VERTICES) {
        vec2 mouse;
        int x;
        int y;
        glfwGetMousePos(&x, &y);
        mouse.x = x;
        mouse.y = y;

        vec3 o = {camera.x, camera.y, camera.z};
        vec3 d = raycast(projection, camera, mouse, viewport);
        mat4 transform;

        glDisable(GL_DEPTH_TEST);
        shader.set_light_color(0.15f, 0.8f, 0.15f); // green
        transform = no_view_scaling_transform(0, 0, 0, {0.4, 0.4, 0.4}, view, 90, 0, 0);
        if(is_mouse_over_arrow(o, d, transform)) {
            shader.set_light_color(1.0f, 0.3f, 0.3f);
        }
        shader.set_transform(transform);
        draw_model(&arrow);

        shader.set_light_color(0.15f, 0.15f, 0.8f); // blue
        transform = no_view_scaling_transform(0, 0, 0, {0.4, 0.4, 0.4}, view, 0, 90, 0);
        if(is_mouse_over_arrow(o, d, transform)) {
            shader.set_light_color(0.3f, 1.0f, 0.3f);
        }
        shader.set_transform(transform);
        draw_model(&arrow);

        shader.set_light_color(0.8f, 0.15f, 0.15f); // red
        transform = no_view_scaling_transform(0, 0, 0, {0.4, 0.4, 0.4}, view, 0, 0, 90);
        if(is_mouse_over_arrow(o, d, transform)) {
            shader.set_light_color(0.3f, 0.3f, 1.0f);
        }
        shader.set_transform(transform);
        draw_model(&arrow);
        glEnable(GL_DEPTH_TEST);
    }

    //Draw lines to show the axis of the 3d grid
    shader.set_light_color(1.0f, 0.3f, 0.3f);
    shader.set_transform(no_view_scaling_transform(0, 0, 0, {10000, 0.04, 0.04}, view));
    draw_model(&cylinderModel);

    shader.set_light_color(0.3f, 1.0f, 0.3f);
    shader.set_transform(no_view_scaling_transform(0, 0, 0, {0.04, 0.04, 10000}, view));
    draw_model(&cylinderModel);

    shader.set_light_color(0.3f, 0.3f, 1.0f);
    shader.set_transform(no_view_scaling_transform(0, 0, 0, {0.04, 10000, 0.04}, view));
    draw_model(&cylinderModel);

    if(state == STATE_SELECT_VERTICES) {
        bshader.bind();
        bshader.set_view(view);
        entities[selectedEntity].draw_vertices(bshader, &billboard, circle, view, {camera.x, camera.y, camera.z});
        //for (Entity &e : entities) {
        //    e.draw_vertices(bshader, &billboard, circle, view, {camera.x, camera.y, camera.z});
        //}
    }
}

void MeshEditor::set_camera(float zoom, float x, float y, float z, float yaw, float pitch, float roll) {
    camera = {x, y, z, pitch, yaw, roll};
}

void MeshEditor::add_model(const char* str) {
    entities.emplace_back();
    entities.back().load(str);
    printf("added model\n");
}

char* MeshEditor::export_model(int ID, const char* fileformat) {
    Assimp::Exporter exporter;
    const aiScene *scene;
    aiMesh mesh;
    mesh.mNormals;
    mesh.mVertices;
    mesh.mFaces[0].mIndices;
    scene->mMeshes[0] = &mesh;
    const aiExportDataBlob* blob = exporter.ExportToBlob(scene, "obj", 0);
    return (char*)blob->data;
}

void MeshEditor::on_mouse_up(int x, int y, int x2, int y2) {
    for(Entity& e: entities) {
        e.select(x, y, x2, y2, camera, projection, viewport);
    }

#if 0
    int width = x2-x;
    int height = y2-y;

    mat4 view = create_view_matrix(camera);
    bind_framebuffer(pickbuffer);
    glClearColor(u32_to_rgba(NULL_PICK).x,u32_to_rgba(NULL_PICK).y, u32_to_rgba(NULL_PICK).z, u32_to_rgba(NULL_PICK).w);
    glClear(GL_COLOR_BUFFER_BIT);
    pshader.bind();
    pshader.set_view(view);
    for (Entity &e : entities) {
        e.reset_selected_vertices();
        e.draw_vertices(pshader, &billboard, circle, view, {camera.x, camera.y, camera.z});

        u32 pick = NULL_PICK;

        u8 res[width*height*4];
        vec2 mouse;
        int mousex;
        int mousey;
        glfwGetMousePos(&mousex, &mousey);
        mouse.x = mousex;
        mouse.y = mousey;
        glReadPixels(x, y, x2, y2, GL_RGBA, GL_UNSIGNED_BYTE, &res);

        for(int i = 0; i < width*height*4; i+=4) {
            pick = rgba_to_u32(res[i+0], res[i+1], res[i+2], res[i+3]);
            printf("pick: %u\n", pick);

            e.set_vertex_ID_selected(pick);
        }

        unbind_framebuffer();
    }
    glClearColor(0.1f, 0.1f, 0.2f, 0.0f);

#endif
}


// Scale every vertex in every mesh in every entity by the factor passed in
void MeshEditor::scale_all_entities(float factor) {
    for(Entity& e: entities) {
        e.scale_entity(factor);
    }
}

bool MeshEditor::is_mouse_over_arrow(vec3 o, vec3 d, mat4 transform) {
    for(Mesh m : arrow.meshes) {
        for(int i = 0; i < m.indices.size(); i+=3) {
            int index1 = m.indices[i+0];
            int index2 = m.indices[i+1];
            int index3 = m.indices[i+2];
            Vertex one = m.vertices[index1];
            Vertex two = m.vertices[index2];
            Vertex three = m.vertices[index3];

            vec3 collisionPoint;

            if(ray_tri_collision(
                    o, d,
                    (transform * V4(one.position.x, one.position.y, one.position.z, 1.0)).xyz,
                    (transform * V4(two.position.x, two.position.y, two.position.z, 1.0)).xyz,
                    (transform * V4(three.position.x, three.position.y, three.position.z, 1.0)).xyz,
                    &collisionPoint
            )
                    ) {
                return true;
            }
        }
    }

    return false;
}

MeshEditor::~MeshEditor() {
    shader.dispose();
    bshader.dispose();
}