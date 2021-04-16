#include <iostream>
#include <fstream>
#include "MeshEditor.h"
#include "StairsString.h"
#include "assimp/Exporter.hpp"

MeshEditor::MeshEditor() {
    shader.load();
    camera = {0};
    entities.emplace_back(staircaseobjhardcoded);

    export_new(1, "obj");
}

void MeshEditor::run() {
    local float rotation = 0.0f;
    rotation+=0.2f;
    local float zoom = 5.0f;
    //zoom-=0.025f;

    shader.bind();
    shader.set_light_pos(14, 14, 14);
    shader.set_light_color(147.0f/255.0f, 108.0f/255.0f,95.0f/255.0f);
    shader.set_camera_pos(zoom, zoom, zoom);
    shader.set_view(look_at({zoom, zoom, zoom}, {0, 0, 0}));

    for(Entity& e : entities) {
        e.draw(shader);
        e.set_rotation( {rotation, rotation, rotation} );
    }
}

void MeshEditor::set_camera(float zoom, float x, float y, float z, float yaw, float pitch, float roll){
    camera = {x, y, z, pitch, yaw, roll};
}

void MeshEditor::add_model(const char* str) {
    entities.emplace_back(str);
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

// Scale every vertex in every mesh in every entity by the factor passed in
void MeshEditor::scale_all_entities(float factor) {
    for(Entity& e: entities) {
        e.scale_entity(factor);
    }
}

std::string MeshEditor::export_new(int ID, const char *fileformat) {
    std::string modelData;
    //get the current model
    for (Entity e: entities) {
        Model curr = e.get_current();
        for (Mesh m: curr.meshes) {
            int j = 0;
            //loop through the model and stringify, starting with the vertices
            for (Vertex v: m.vertices) {
                float verticesX = m.vertices[j].position.x;
                float verticesY = m.vertices[j].position.y;
                float verticesZ = m.vertices[j].position.z;
                std::string line = "v " + std::to_string(verticesX) + " " + std::to_string(verticesY) + " " + std::to_string(verticesZ) + "\n";
                modelData.append(line);
                j++;
            }
            j = 0;
            //texture coordinates
            for (Vertex v: m.vertices) {
                float texturesX = m.vertices[j].uv.x;
                float texturesY = m.vertices[j].uv.y;
                std::string line = "vt " + std::to_string(texturesX) + " " + std::to_string(texturesY) + "\n";
                modelData.append(line);
                j++;
            }
            j = 0;
            //vertex normals
            for (Vertex v: m.vertices) {
                float normalsX = m.vertices[j].normal.x;
                float normalsY = m.vertices[j].normal.y;
                float normalsZ = m.vertices[j].normal.z;
                std::string line = "vn " + std::to_string(normalsX) + " " + std::to_string(normalsY) + " " + std::to_string(normalsZ) + "\n";
                modelData.append(line);
                j++;
            }
            //now the faces, which needs work
            for (j = 0; j < m.indices.size(); j+=3) {
                int face1 = m.indices[j];
                int face2 = m.indices[j + 1];
                int face3 = m.indices[j + 2];
                std::string line = "f " + std::to_string(face1) + " " + std::to_string(face2) + " " + std::to_string(face3) + "\n";
                modelData.append(line);
                j++;
            }
        }
    }

    std::ofstream testfile;
    testfile.open("C:/Users/Phil/Desktop/testfile.txt");
    if (testfile.is_open()) {
        std::cout << "We made it, textfile should exist\n";
        testfile << modelData;
    }
    else
        std::cout << "Fix your pathing, it did not create the file\n";
    testfile.close();
    //return the string containing the vertices and faces of the model
    return modelData;
}

MeshEditor::~MeshEditor() {
    shader.dispose();
}