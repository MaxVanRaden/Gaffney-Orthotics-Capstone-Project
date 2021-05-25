#include <iostream>
#include <fstream>
#include "MeshEditor.h"
#include "StairsString.h"
#include "CylinderString.h"
#include "ArrowString.h"
#include "assimp/Exporter.hpp"
#include "emscripten.h"

enum axis {
    x,
    y,
    z
};

extern "C" {

    EM_JS(float, get_translation_factor, (), {
        return document.getElementById('translationFactor').value;
    });

    EM_JS(bool, is_select_or_move_checked, (), {
        return (document.getElementById('selectToggle').checked || document.getElementById('moveToggle').checked);
    });

}

MeshEditor::MeshEditor() {
    axis_clicked = false;
    export_strlen = 0;
    shader.load();
    bshader.load();
    pshader.load();
    //TODO: [DEV] Change back to 0
    camera = {0, 0, 0, 0, 0, 0};
    fliparrows = false;

    entities.emplace_back();
    redostack.clear();
    undostack.emplace_back(entities.back());
    //TODO: [DEV] Comment out staircaseobj
    entities.back().load(staircaseobjhardcoded, 0);
    entities.back().set_relative_position({50, 50, -350});
    projection = perspective_projection(90, 16.0f / 9.0f, 0.01f, 3000.0f);
    move_cam_backwards(&camera, 10);
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

    cylinderModel = load_model_string(cylinderHardcoded, 0);
    arrow = load_model_string(arrowHardcoded, 0);
    arrow.pos = calculate_avg_pos_selected_vertices();

    camera.x -= 5;
    camera.y -= 5;
    mouseDown = false;

    crossSectionBot = crossSectionTop = INVALID_CROSS_SECTION;
    placedFirstSection = false;

    //pickbuffer = create_color_buffer(1920, 1080, GL_LINEAR);
}

void MeshEditor::run(int width, int height) {
    viewport = {0, 0, (float)width, (float)height};

    int button = glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT);
    if(button == GLFW_PRESS) {
        mouseDown = true;
    }
    if(button == GLFW_RELEASE) {
        mouseDown = false;
    }

    if(state == STATE_SELECT_ENTITY) {
        int keystate = glfwGetKey(KEY_C);
        if(keystate == GLFW_PRESS) {
            state = STATE_SELECT_CROSS_SECTION;
        }

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
        int keystate = glfwGetKey(GLFW_KEY_ESC);
        if(keystate == GLFW_PRESS) {
            state = STATE_SELECT_ENTITY;
        }
    } else if(state == STATE_SELECT_CROSS_SECTION) {
        int keystate = glfwGetKey(GLFW_KEY_ESC);
        if(keystate == GLFW_PRESS) {
            state = STATE_SELECT_ENTITY;
        }
    }

    if(state == STATE_SELECT_CROSS_SECTION) {
        //selectedEntity = -1;
        vec2 mouse;
        int x;
        int y;
        glfwGetMousePos(&x, &y);
        mouse.x = x;
        mouse.y = y;

        vec3 rayposition = {camera.x, camera.y, camera.z};
        vec3 raydirection = raycast(projection, camera, mouse, viewport);

        int buttonLeft = glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT);
        int buttonRight = glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT);

        for (int i = 0; i < entities.size(); ++i) {
            if(placedFirstSection) {
                crossSectionTop = entities[i].place_line(rayposition, raydirection);
                if(buttonRight == GLFW_PRESS) {
                    placedFirstSection = false;
                    crossSectionTop = INVALID_CROSS_SECTION;
                    crossSectionBot = INVALID_CROSS_SECTION;
                }
                //if(buttonLeft == GLFW_PRESS) {
                int keystate = glfwGetKey(GLFW_KEY_ENTER);
                if(keystate == GLFW_PRESS) {
                    entities[i].select_vertices_in_cross_section(crossSectionBot, crossSectionTop);
                    state = STATE_SELECT_VERTICES;
                }
                //}
            }
            else {
                if(buttonLeft == GLFW_PRESS) {
                    placedFirstSection = true;
                }
                crossSectionBot = entities[i].place_line(rayposition, raydirection);
            }
        }
    }

    showOverlay = false;

//    camera.x+=0.02f;
//    camera.y+=0.02f;
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

    if(state == STATE_SELECT_CROSS_SECTION) {
        shader.set_show_cross_section(true);
        shader.set_cross_section_bot(crossSectionBot);
        shader.set_cross_section_top(crossSectionTop);
    } else {
        shader.set_show_cross_section(false);
    }

    for(Entity& e : entities) {
        e.draw(shader);
    }
    shader.set_show_cross_section(false);

    if(showOverlay) {
        for (Entity &e : entities) {
            e.draw_overlay(shader);
        }
    }

    shader.set_solid_color(true);
    if(state == STATE_SELECT_VERTICES) {
        vec2 mouse;
        Axis axis;
        int x;
        int y;
        glfwGetMousePos(&x, &y);
        mouse.x = x;
        mouse.y = y;

        vec3 o = {camera.x, camera.y, camera.z};
        vec3 d = raycast(projection, camera, mouse, viewport);
        mat4 transform;

        // Average position of selected vertices
        float avgX = 0.0f;
        float avgY = 0.0f;
        float avgZ = 0.0f;
        float total = 0.0f;

        //TODO: Make all three arrows not flat

        glDisable(GL_DEPTH_TEST);
        shader.set_light_color(0.15f, 0.8f, 0.15f); // green
        if(fliparrows)
            transform = no_view_scaling_transform(arrow.pos.x, arrow.pos.y, arrow.pos.z, {0.4, 0.4, 0.4}, view, 270, 0, 0);
        else
            transform = no_view_scaling_transform(arrow.pos.x, arrow.pos.y, arrow.pos.z, {0.4, 0.4, 0.4}, view, 90, 0, 0);
        if(is_mouse_over_arrow(o, d, transform)) {
            int button = glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT);
            if(button == GLFW_PRESS) {
                mouseDown = true;
                dragDirection = V3(0, 0, 1);
            }
            shader.set_light_color(1.0f, 0.3f, 0.3f);
            if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !axis_clicked) {
                axis_clicked = true;
                axis = Z;
            }
        }
        shader.set_transform(transform);
        draw_model(&arrow);

        shader.set_light_color(0.15f, 0.15f, 0.8f); // blue
        if(fliparrows)
            transform = no_view_scaling_transform(arrow.pos.x, arrow.pos.y, arrow.pos.z, {0.4, 0.4, 0.4}, view, 180, 0, 0);
        else
            transform = no_view_scaling_transform(arrow.pos.x, arrow.pos.y, arrow.pos.z, {0.4, 0.4, 0.4}, view, 0, 90, 0);
        if(is_mouse_over_arrow(o, d, transform)) {
            int button = glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT);
            if(button == GLFW_PRESS) {
                mouseDown = true;
                int lasxpos;
                int lasypos;
                glfwGetMousePos(&lasxpos, &lasypos);
                dragDirection = V3(0, 1, 0);
            }
            shader.set_light_color(0.3f, 1.0f, 0.3f);
            if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !axis_clicked) {
                axis_clicked = true;
                axis = Y;
            }
        }
        shader.set_transform(transform);
        draw_model(&arrow);

        shader.set_light_color(0.8f, 0.15f, 0.15f); // red
        if(fliparrows)
            transform = no_view_scaling_transform(arrow.pos.x, arrow.pos.y, arrow.pos.z, {0.4, 0.4, 0.4}, view, 0, 0, 270);
        else
            transform = no_view_scaling_transform(arrow.pos.x, arrow.pos.y, arrow.pos.z, {0.4, 0.4, 0.4}, view, 0, 0, 90);
        if(is_mouse_over_arrow(o, d, transform)) {
            int button = glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT);
            if(button == GLFW_PRESS) {
                mouseDown = true;
                dragDirection = V3(1, 0, 0);
            }
            shader.set_light_color(0.3f, 0.3f, 1.0f);
            if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !axis_clicked) {
                axis_clicked = true;
                axis = X;
            }
        }
        shader.set_transform(transform);
        draw_model(&arrow);
        glEnable(GL_DEPTH_TEST);

        if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && axis_clicked && !is_select_or_move_checked()) {
            translate_vertices_along_axis(axis);
        } else {
            if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS) {
                axis_clicked = false;
                //TODO: Consider this a complete operation, so save the model here
            }
        }
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

    shader.set_solid_color(false);

    if(state == STATE_SELECT_VERTICES) {
        bshader.bind();
        bshader.set_view(view);
        entities[selectedEntity].draw_vertices(bshader, &billboard, circle, view, {camera.x, camera.y, camera.z});
        //for (Entity &e : entities) {
        //    e.draw_vertices(bshader, &billboard, circle, view, {camera.x, camera.y, camera.z});
        //}
    }
}

//Translates vertices along the passed in axis if the left mouse button is being pressed.
void MeshEditor::translate_vertices_along_axis(Axis axis) {
    //Z axis is flipped for some reason
    float translation_factor;
    if (axis == Z)
        translation_factor = fliparrows ? get_translation_factor() : (-1) * get_translation_factor();
    else
        translation_factor = fliparrows ? (-1) * get_translation_factor() : get_translation_factor();

    //Loop over every selected vertex and move appropriate axis by the translation factor
    for (Entity& e : entities) {
        for (Mesh& m : e.get_current().meshes) {
            for (u32 index: m.selected_vertices) {
                switch (axis) {
                    case X: m.vertices[index].position.x += translation_factor; break;
                    case Y: m.vertices[index].position.y += translation_factor; break;
                    case Z: m.vertices[index].position.z += translation_factor; break;
                }
            }
            switch (axis) {
                case X: arrow.pos.x += translation_factor; break;
                case Y: arrow.pos.y += translation_factor; break;
                case Z: arrow.pos.z += translation_factor; break;
            }
            glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m.vertices.size(), &m.vertices[0], GL_STATIC_DRAW);
        }
    }
}

void MeshEditor::set_camera(float zoom, float x, float y, float z, float yaw, float pitch, float roll) {
    camera = {x, y, z, pitch, yaw, roll};
    move_cam_backwards(&camera, zoom);
}

void MeshEditor::add_model(const char* str, int fileformat) {
    redostack.clear();
    undostack.clear();
    entities.clear();
    entities.emplace_back();
    entities.back().load(str, fileformat);
    entities.back().set_relative_position({4, 4, 4});
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
    for(Entity& e: entities) {
        e.select(x, y, x2, y2, camera, projection, viewport);
    }
    arrow.pos = calculate_avg_pos_selected_vertices();
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

vec3 MeshEditor::calculate_avg_pos_selected_vertices() {
    vec3 pos = {0};

    // Average position of selected vertices
    float avgX = 0.0f;
    float avgY = 0.0f;
    float avgZ = 0.0f;
    float total = 0.0f;

    for (Entity& e : entities){
        for(Mesh& m : e.get_current().meshes){
            for(u32 index : m.selected_vertices) {
                avgX += m.vertices[index].position.x;
                avgY += m.vertices[index].position.y;
                avgZ += m.vertices[index].position.z;
                total += 1.0f;
            }
        }
    }

    avgX /= total;
    avgY /= total;
    avgZ /= total;
    pos.x = avgX;
    pos.y = avgY;
    pos.z = avgZ;

    return pos;
}


// Scale every vertex in every mesh in every entity by the factor passed in
void MeshEditor::scale_all_entities(float factor) {
    set_undo(); // adds to the undo stack & resets the redo stack
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

// Getter function to return the size (in bytes) of the export string
//      will return 0 if export_model has not been called yet.
//      Seems dumb, but necessary because we have to pass to front-end in a weird way.
//      Talk to Jacob if you feel like you have a better solution and I'll hear you out.
uint32_t MeshEditor::get_export_strlen() const {
    return export_strlen;
}

// created for code reusability
void MeshEditor::set_undo() {
    redostack.clear();
    if(undostack.size() < 25 )
        undostack.emplace_back(entities.back());
}

void MeshEditor::undo_model() {
    if(!undostack.empty()) {
        Entity revert = undostack.back(); // grab the earlier used state from the design
        if(redostack.size() < 25 )
            redostack.emplace_back(revert); // push top of undo stack to redo before popping it
        entities.pop_back(); // pop latest change from design
        if (revert.get_current().meshes.size() != 0 || // checks for content before adding
            (revert.get_current().materials.size() != 0)) {
            entities.emplace_back(revert);
        }
        // refresh screen with changes:
        for (Entity &e : entities) {
            for (Mesh &m : e.get_current().meshes) {
                glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m.vertices.size(), &m.vertices[0], GL_STATIC_DRAW);
            }
        }
        undostack.pop_back(); // pop whatever was on the undo stack
    }
    printf("undo function end: ");
    printf("%d undostack, ", undostack.size());
    printf("%d redostack, ", redostack.size());
    printf("%d entities\n\n", entities.size());
}

//TODO: may still contain an alignment fault, needs more testing
void MeshEditor::redo_model() {
    if (!redostack.empty()) {
        // checks for content before adding:
        if (redostack.back().get_current().meshes.size() != 0 ||
            (redostack.back().get_current().materials.size() != 0)) {
            undostack.emplace_back(redostack.back());     // update undo stack with change
            redostack.pop_back(); // pop whatever was on the redo stack
            if (!redostack.empty()) {
                // checks for content before adding:
                if (redostack.back().get_current().meshes.size() != 0 ||
                    (redostack.back().get_current().materials.size() != 0)) {
                    Entity revert = redostack.back();   // grab from redo stack
                    entities.pop_back();                // remove last modification (should be first undo)
                    entities.emplace_back(revert);      // update
                }
            }
            // refresh screen with changes:
            for (Entity &e : entities) {
                for (Mesh &m : e.get_current().meshes) {
                    glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m.vertices.size(), &m.vertices[0], GL_STATIC_DRAW);
                }
            }
        }
    }
    printf("redo function end: ");
    printf("%d undostack, ", undostack.size());
    printf("%d redostack, ", redostack.size());
    printf("%d entities\n\n", entities.size());
}

void MeshEditor::flip_axis() {
    if(fliparrows)
        fliparrows = false;
    else
        fliparrows = true;
}

MeshEditor::~MeshEditor() {
    shader.dispose();
    bshader.dispose();
}