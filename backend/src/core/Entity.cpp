#include "Entity.h"

Entity::Entity() {

}

Entity::Entity(std::string file) {
    current = load_model_string(file);
    start = load_model_string(file);
}

void Entity::draw(StaticShader& shader) {
    mat4 transform = create_transformation_matrix( current.pos, current.rotate, current.scale );
    shader.set_transform(transform);

    draw_model(&current);
}

void Entity::set_position(vec3 pos) {
    current.pos = pos;
}

void Entity::set_rotation(vec3 rotate) {
    current.rotate = rotate;
}

void Entity::set_scale(vec3 scale) {
    current.scale = scale;
}

Entity::~Entity() {
    dispose_model(&current);
    dispose_model(&start);
}