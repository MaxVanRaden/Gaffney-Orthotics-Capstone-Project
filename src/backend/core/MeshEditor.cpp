#include "MeshEditor.h"
#include "StairsString.h"

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
    shader.set_light_color(147.0f/255.0f, 108.0f/255.0f,95.0f/255.0f);
    shader.set_view(look_at({zoom, zoom, zoom}, {0, 0, 0}));

    for(Entity& e : entities) {
        e.draw(shader);
        e.set_rotation( {rotation, rotation, rotation} );
    }
}

MeshEditor::~MeshEditor() {
    shader.dispose();
}