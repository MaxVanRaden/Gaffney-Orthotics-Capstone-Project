#include <iostream>
#include <fstream>
#include "MeshEditor.h"
#include "StairsString.h"
#include "CylinderString.h"
#include "assimp/Exporter.hpp"

MeshEditor::MeshEditor() {
    export_strlen = 0;
    shader.load();
    bshader.load();
    pshader.load();
    //TODO: [DEV] Change back to 0
    camera = {3, 6, 0, 0, 0, 0};

    entities.emplace_back();
    //TODO: [DEV] Comment out staircaseobj
    entities.back().load(staircaseobjhardcoded, 0);
    entities.back().set_position( {4, 4, 4} );
    redostack.clear();
    undostack.emplace_back(entities.back());
    projection = perspective_projection(90, 16.0f / 9.0f, 0.01f, 3000.0f);
    move_cam_backwards(&camera, 10);

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

    cylinderModel = load_model_string(cylinderHardcoded, 0);

    camera.x -= 5;
    camera.y -= 5;

    //pickbuffer = create_color_buffer(1920, 1080, GL_LINEAR);

    //std::thread test(thread_test);
    //test.join();
}

void MeshEditor::run(int width, int height) {
    viewport = {0, 0, (float)width, (float)height};

    local float rotation = 0.0f;
    //rotation+=0.2f;
    local float zoom = 5.0f;
    //zoom-=0.025f;

#if 0
    vec2 mouse;
    int x;
    int y;
    glfwGetMousePos(&x, &y);
    mouse.x = x;
    mouse.y = y;

    vec3 rayposition = {camera.x, camera.y, camera.z};
    vec3 raydirection = raycast(projection, camera, mouse, viewport);

    for(int i = 0; i < entities.size(); ++i) {
        if(entities[i].is_mouse_over(rayposition, raydirection)) {
            printf("mouse is over a mesh\n");
        } else {
            printf("mouse is not over a mesh\n");
        }
    }
#endif

//    camera.x+=0.02f;
//    camera.y+=0.02f;
    mat4 view = create_view_matrix(camera);

    shader.bind();
    shader.set_light_pos(14, 14, 14);
    shader.set_light_color(147.0f/255.0f, 108.0f/255.0f,95.0f/255.0f);
    shader.set_camera_pos(camera.x, camera.y, camera.z);
    shader.set_view(view);

    static int timer=0;
    timer++;

    for(Entity& e : entities) {
        e.draw(shader);
        e.set_rotation( {rotation, rotation, rotation} );
    }

    //Draw lines to show the axis of the 3d grid
    shader.set_light_color(1.0f, 0.3f, 0.3f);
    shader.set_transform(no_view_scaling_transform(0, 0, 0, {100, 0.04, 0.04}, view));
    draw_model(&cylinderModel);

    shader.set_light_color(0.3f, 1.0f, 0.3f);
    shader.set_transform(no_view_scaling_transform(0, 0, 0, {0.04, 0.04, 100}, view));
    draw_model(&cylinderModel);

    shader.set_light_color(0.3f, 0.3f, 1.0f);
    shader.set_transform(no_view_scaling_transform(0, 0, 0, {0.04, 100, 0.04}, view));
    draw_model(&cylinderModel);

    bshader.bind();
    bshader.set_view(view);
    for(Entity& e : entities) {
        e.draw_vertices(bshader, &billboard, circle, view, {camera.x, camera.y, camera.z});
    }
}

void MeshEditor::set_camera(float zoom, float x, float y, float z, float yaw, float pitch, float roll){
    camera = {x, y, z, pitch, yaw, roll};
}

void MeshEditor::add_model(const char* str, int fileformat) {
    entities.emplace_back();
    entities.back().load(str, fileformat);
    redostack.clear();
    undostack.emplace_back(entities.back());
    printf("added model\n");
}

// Returns char* to either a valid .obj/.stl string or null
// Sets this->export_strlen to length of that string (not including null terminator)
//      which can be retrieved via get_export_strlen()
// Accepted fileformat parameters: ".obj", ".stl"
char* MeshEditor::export_model(const char* fileformat) {
    if (!strcmp(fileformat, ".obj")) {
        std::string modelData;
        //get the current model
        for (Entity& e : entities) {
            Model curr = e.get_current();
            for (Mesh& m : curr.meshes) {
                int j = 0;
                //loop through the model and stringify, starting with the vertices
                for (Vertex& v : m.vertices) {
                    float verticesX = m.vertices[j].position.x;
                    float verticesY = m.vertices[j].position.y;
                    float verticesZ = m.vertices[j].position.z;
                    std::string line = "v " + std::to_string(verticesX) + " " + std::to_string(verticesY) + " " + std::to_string(verticesZ) + "\n";
                    modelData.append(line);
                    j++;
                }

                j = 0;
                // Texture Coordinates
                for (Vertex& v : m.vertices) {
                    float texturesX = m.vertices[j].uv.x;
                    float texturesY = m.vertices[j].uv.y;
                    std::string line = "vt " + std::to_string(texturesX) + " " + std::to_string(texturesY) + "\n";
                    modelData.append(line);
                    j++;
                }
                j = 0;

                // Vertex Normals
                for (Vertex v : m.vertices) {
                    float normalsX = m.vertices[j].normal.x;
                    float normalsY = m.vertices[j].normal.y;
                    float normalsZ = m.vertices[j].normal.z;
                    std::string line = "vn " + std::to_string(normalsX) + " " + std::to_string(normalsY) + " " + std::to_string(normalsZ) + "\n";
                    modelData.append(line);
                    j++;
                }

                // Faces
                for (j = 0; j < m.indices.size(); j += 3) {
                    int face1 = m.indices[j] + 1;
                    int face2 = m.indices[j + 1] + 1;
                    int face3 = m.indices[j + 2] + 1;
                    std::string line = "f " + std::to_string(face1) + " " + std::to_string(face2) + " " + std::to_string(face3) + "\n";
                    modelData.append(line);
                }
            }
        }

        // Return the string containing the vertices and faces of the model
        char *temp = new char[modelData.size()];
        strcpy(temp, modelData.c_str());

        // Set export_strlen
        export_strlen = modelData.size();
        return temp; // yes memory leak, implement fix if you can find one
    } else if (!strcmp(fileformat, ".stl")){
        std::string modelData;
        //get the current model
        for (Entity& e : entities) {
            Model curr = e.get_current();
            for (Mesh& m : curr.meshes) {
                std::string header = "OrthoFreeD STLWriter\n";
                modelData.append(header);
                int j = 0;
                //loop through the model and stringify, starting with the vertices
                for(int i = 0; i < m.vertices.size()/3; i++) {
                    float verticesX = m.vertices[j].position.x;
                    float verticesY = m.vertices[j].position.y;
                    float verticesZ = m.vertices[j].position.z;
                    std::string v1 =
                            "facet normal 0.0 0.0 0.0\n  outer loop\n    vertex " +
                            std::to_string(verticesX) +
                            "  " +
                            std::to_string(verticesY) +
                            "  " +
                            std::to_string(verticesZ) +
                            "\n";
                    modelData.append(v1);

                    j++;
                    verticesX = m.vertices[j].position.x;
                    verticesY = m.vertices[j].position.y;
                    verticesZ = m.vertices[j].position.z;
                    std::string v2 =
                            "    vertex " +
                            std::to_string(verticesX) +
                            " " +
                            std::to_string(verticesY) +
                            " " +
                            std::to_string(verticesZ) +
                            "\n";
                    modelData.append(v2);

                    j++;
                    verticesX = m.vertices[j].position.x;
                    verticesY = m.vertices[j].position.y;
                    verticesZ = m.vertices[j].position.z;
                    std::string v3 =
                            "    vertex " +
                            std::to_string(verticesX) +
                            " " +
                            std::to_string(verticesY) +
                            " " +
                            std::to_string(verticesZ) +
                            "\n  endloop\nendfacet\n";
                    modelData.append(v3);
                    j++;
                }
                std::string footer = "endsolid OrthoFreeD STLWriter";
                modelData.append(footer);
            }
        }

        // Return the string containing the vertices and faces of the model
        char *temp = new char[modelData.size()];
        strcpy(temp, modelData.c_str());

        // Set export_strlen
        export_strlen = modelData.size();
        return temp; // yes memory leak, implement fix if you can find one
    } else {
        return nullptr;
    }
}

void MeshEditor::on_mouse_up(int x, int y, int x2, int y2) {

    printf("mosue up\n");
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
    redostack.clear();
    undostack.emplace_back(entities.back());
}

// Getter function to return the size (in bytes) of the export string
//      will return 0 if export_model has not been called yet.
//      Seems dumb, but necessary because we have to pass to front-end in a weird way.
//      Talk to Jacob if you feel like you have a better solution and I'll hear you out.
uint32_t MeshEditor::get_export_strlen() const {
    return export_strlen;
}

//TODO: [WIP] Naive translate moves in x direction by one unit
void MeshEditor::translate_vertex() {
    for (Entity& e : entities) {
        for (Mesh &m : e.get_current().meshes) {
            for(u32 index: m.selected_vertices){
                m.vertices[index].position.x += 1;
            }
            glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m.vertices.size(), &m.vertices[0], GL_STATIC_DRAW);
        }
    }
    redostack.clear();
    undostack.emplace_back(entities.back());
    return;
}

//TODO: if two models receive changes simultaneously, only the most recently added model can be undone
//TODO: cannot undo any state changes that are made before using translate_vertex
void MeshEditor::undo_model() {
    if(!undostack.empty()) {
        redostack.emplace_back(undostack.back());
        undostack.pop_back();
        entities.pop_back();
        if(!undostack.empty()){
            entities.emplace_back(undostack.back());
        } else if (entities.size() == 1){
            undostack.emplace_back(entities.back());
            entities.emplace_back();
        } else if (entities.size() == 0){
            entities.emplace_back();
        }
        // Update the VBO buffer to reflect any changes in vertices
        for (Entity& e : entities) {
            for (Mesh &m : e.get_current().meshes) {
                glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m.vertices.size(), &m.vertices[0], GL_STATIC_DRAW);
            }
        }
    }
    printf("%d undostack size\n", undostack.size());
}

void MeshEditor::redo_model() {
    //TODO: test implementation of redo function
    if(!redostack.empty()) {
        Entity curr = redostack.back();
        redostack.pop_back();
        entities.emplace_back(curr);
        undostack.emplace_back(curr);
        // Update the VBO buffer to reflect any changes in vertices
        for (Entity& e : entities) {
            for (Mesh &m : e.get_current().meshes) {
                glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m.vertices.size(), &m.vertices[0], GL_STATIC_DRAW);
            }
        }
    }
    printf("%d redostack size\n", redostack.size());
}

MeshEditor::~MeshEditor() {
    shader.dispose();
    bshader.dispose();
}