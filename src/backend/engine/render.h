#ifndef RENDER_H
#define RENDER_H

#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <unordered_map>
#include "shaders.h"

#define INVALID_MATERIAL 0xFFFFFFFF
#define NULL_PICK        0xFFFFFFFF

static inline
u32 rgba_to_u32(u8 r, u8 g, u8 b, u8 a) {
	return ( (r & 0xFF) << 24) + ( (g & 0xFF) << 16) + ( (b & 0xFF) << 8) + ( (a & 0xFF) ); 
}

static inline
vec4 u32_to_rgba(u32 color) {
    vec4 c;
    c.x = ( (color >> 24) & 0xFF);
    c.y = ( (color >> 16) & 0xFF);
    c.z = ( (color >>  8) & 0xFF);
    c.w = ( (color)       & 0xFF);
    return c;
}

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 uv;
};

struct Material {
    Texture diffuse;
    Texture normals;
    Texture specular;
    vec4 diffuseColor;
    vec4 ambientColor;
    vec4 specularColor;
    f32 gloss;
};

struct Mesh {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
	std::vector<Vertex> vertices;
	std::vector<GLushort> indices;
    u32 indexcount;
    u32 material;
};

struct Model {
    std::vector<Mesh> meshes;
    std::vector<Material> materials;

    vec3 pos;
    vec3 rotate;
    vec3 scale;
};  

void dispose_mesh(Mesh* mesh);
void dispose_model(Model* model);
Mesh create_mesh(std::vector<Vertex> vertices, std::vector<GLushort> indices);
void load_mesh(Model* model, u32 i, const aiMesh* paiMesh);
Model load_model(const char* filename);
Model load_model_string(std::string file);
void draw_mesh(Mesh mesh);
void draw_mesh(Mesh mesh, Texture tex);
void draw_model(Model* model);

Mesh create_billboard();
Mesh create_ground_quad(f32 width, f32 height);
void draw_billboard_unordered(Mesh* mesh, Texture texture);
mat4 billboard_transform(f32 x, f32 y, f32 z, vec3 scaleVec, vec3 rotation, mat4& view);
mat4 billboard_transform(f32 x, f32 y, f32 z, vec3 scaleVec, mat4& view);
mat4 billboard_transform(f32 x, f32 y, f32 z, vec3 scaleVec, f32 rotation, mat4& view);

#endif
