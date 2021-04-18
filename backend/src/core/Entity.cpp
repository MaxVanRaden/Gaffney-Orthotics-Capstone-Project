#include "Entity.h"

Entity::Entity() {
    current.scale = current.rotate = current.pos = {0};
    current.materials.clear();
    current.meshes.clear();
    start = current;
}

Entity::Entity(std::string file) {
    //current = load_model_string(file);
    //start = load_model_string(file);
}

void Entity::load(std::string file) {
    file[file.size()] = 0;
    current = load_model_string(file);
    //start = current;
}

void Entity::draw(StaticShader& shader) {
    mat4 transform = create_transformation_matrix( current.pos, current.rotate, current.scale );
    shader.set_transform(transform);

    draw_model(&current);
}

void Entity::draw_vertices(BillboardShader& shader, Mesh* billboard, Texture circle, mat4 view, vec3 campos) {
    mat4 transform = create_transformation_matrix( current.pos, current.rotate, current.scale );

    //glDisable(GL_DEPTH_TEST);
    //glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);

    bind_texture(circle, 0);
    for(Mesh& mesh : current.meshes) {
        for(Vertex& vertex : mesh.vertices) {
            vec3 pos = (transform * V4(vertex.position.x, vertex.position.y, vertex.position.z, 1.0)).xyz;

            //move the circle a little towards the camera so it's not stuck in the mesh and you can see it clearly
            vec3 dir = normalize(campos - pos);
            pos = pos + (0.05*dir);

            //shader.set_transform(create_transformation_matrix(pos.x, pos.y, pos.z, 0, 0, 0, 1, 1,1 ));
            shader.set_transform(billboard_transform(pos.x, pos.y, pos.z, {0.10, 0.10, 0.10}, view));

            draw_billboard_unordered(billboard);
        }
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);
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
    //dispose_model(&current);
    //dispose_model(&start);
}