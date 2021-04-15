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

bool Entity::is_mouse_over(vec3 o, vec3 d) {
    mat4 transform = create_transformation_matrix( current.pos, current.rotate, current.scale );

    for(Mesh m : current.meshes) {
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