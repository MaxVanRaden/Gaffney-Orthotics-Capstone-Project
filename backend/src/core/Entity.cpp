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

// Scale every vertex in every mesh in the entity by the factor passed in
void Entity::scale_entity(float factor) {
    for (Mesh& mesh : current.meshes) {
        for (Vertex& vertex : mesh.vertices){
            // Scale each vertex's position by the factor
            vertex.position.x *= factor;
            vertex.position.y *= factor;
            vertex.position.z *= factor;
        }
        // Update the VBO buffer to reflect changes in vertices
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh.vertices.size(), &mesh.vertices[0], GL_STATIC_DRAW);
    }
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