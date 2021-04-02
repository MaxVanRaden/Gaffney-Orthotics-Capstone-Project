#include "MeshEditor.h"
#include "StairsString.h"
#include "assimp/Exporter.hpp"

MeshEditor::MeshEditor() {
    shader.load();
    camera = {0};
    entities.emplace_back(staircaseobjhardcoded);
}

void MeshEditor::run() {
    local float rotation = 0.0f;
    rotation+=0.2f;
    local float zoom = 7.0f;
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

MeshEditor::~MeshEditor() {
    shader.dispose();
}