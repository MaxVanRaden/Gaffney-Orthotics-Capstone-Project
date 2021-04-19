#include "render.h"
#include <GL/glfw.h>
#include <GLES2/gl2.h>

void dispose_mesh(Mesh* mesh) {
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteBuffers(1, &mesh->ebo);
    mesh->vbo = mesh->ebo = 0;
    mesh->vertices.clear();
    mesh->indices.clear();
    mesh->indexcount = mesh->material = 0;
    mesh->selected.clear();
    printf("dispose mesh\n");
}

void dispose_model(Model* model) {
    for (u32 i = 0; i < model->meshes.size(); ++i)
        dispose_mesh(&model->meshes[i]);
    model->meshes.clear();
    model->materials.clear();
}

Mesh create_mesh(std::vector<Vertex> vertices, std::vector<GLushort> indices) {
    Mesh mesh = {0};

    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)0);                     //position
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(3 * sizeof(GLfloat))); //normals
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(6 * sizeof(GLfloat))); //tex coords

    glGenBuffers(1, &mesh.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indices.size(), &indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    mesh.indexcount = indices.size();

    return mesh;
}

void load_mesh(Model* model, u32 i, const aiMesh* paiMesh) {
    model->meshes[i].material = paiMesh->mMaterialIndex;

    std::vector<Vertex> vertices;
    std::vector<GLushort> indices;
    std::vector<bool> selected;

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for(u32 i = 0; i < paiMesh->mNumVertices; ++i) {
        const aiVector3D* pos = &(paiMesh->mVertices[i]);
        const aiVector3D* normal = &(paiMesh->mNormals[i]);
        const aiVector3D* uv = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

        Vertex v = {
            {pos->x, pos->y, pos->z},
            {normal->x, normal->y, normal->z},
            {uv->x, uv->y}
        };

        vertices.push_back(v);
        selected.push_back(true);
    }

    for(u32 i = 0; i < paiMesh->mNumFaces; ++i) {
        const aiFace& face = paiMesh->mFaces[i];
        assert(face.mNumIndices == 3);
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }
	
    glGenBuffers(1, &model->meshes[i].vbo);
    glBindBuffer(GL_ARRAY_BUFFER, model->meshes[i].vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)0);                     //vertices
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(3 * sizeof(GLfloat))); //normals
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(6 * sizeof(GLfloat))); //tex coords

    glGenBuffers(1, &model->meshes[i].ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->meshes[i].ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indices.size(), &indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    model->meshes[i].indexcount = indices.size();
    model->meshes[i].vertices = vertices;
    model->meshes[i].selected = selected;
    model->meshes[i].indices = indices;
}

void load_materials(Model* model, const aiScene* pScene, const char* filename) {
    for(u32 i = 0; i < pScene->mNumMaterials; ++i) {
        const aiMaterial* mat = pScene->mMaterials[i];
        model->materials[i] = {0};

        //diffuse
        if(mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString path;

            if(mat->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                std::string fullpath = "data/art/";
                fullpath.append(path.data);
                model->materials[i].diffuse = load_texture(fullpath.c_str(), GL_LINEAR);
            }
        }
        aiColor4D diffuseColor;
        if(aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuseColor) == AI_SUCCESS)
            model->materials[i].diffuseColor = {(f32)diffuseColor.r, (f32)diffuseColor.g, (f32)diffuseColor.b, (f32)diffuseColor.a};

        //specular
        if(mat->GetTextureCount(aiTextureType_SPECULAR) > 0) {
            aiString path;

            if(mat->GetTexture(aiTextureType_SPECULAR, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                std::string fullpath = "data/art/";
                fullpath.append(path.data);
                model->materials[i].specular = load_texture(fullpath.c_str(), GL_LINEAR);
            }
        }
        aiColor4D specularColor;
        if(aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &specularColor) == AI_SUCCESS)
            model->materials[i].specularColor = {(f32)specularColor.r, (f32)specularColor.g, (f32)specularColor.b, (f32)specularColor.a};

        //ambient
        aiColor4D ambientColor;
        if(aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT, &ambientColor) == AI_SUCCESS)
            model->materials[i].ambientColor = {(f32)ambientColor.r, (f32)ambientColor.g, (f32)ambientColor.b, (f32)ambientColor.a};
    }
}

#include <assimp/cimport.h>
Model load_model_string(std::string file) {
    Model model;
    model.pos = {0};
    model.rotate = {0};
    model.scale = {1, 1, 1};

    Assimp::Importer importer;
    const aiScene* pScene = importer.ReadFileFromMemory(
            (void*)&file[0], file.size(), aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_Triangulate |aiProcess_FindInvalidData | aiProcess_ValidateDataStructure | 0, ".obj"
            );

    if(pScene) {
        model.meshes.resize(pScene->mNumMeshes);
        model.materials.resize(pScene->mNumMaterials);
        //for(int i = 0; i < pScene->mNumMeshes+1; i++){
        //    model.meshes.push_back({0});
        //}

        for(u32 i = 0; i < pScene->mNumMeshes; ++i) {
            aiMesh* paiMesh = pScene->mMeshes[i];
            load_mesh(&model, i, paiMesh);
        }
    }
    else {
        printf("Load_model_string: Error loading model %s\n", file.c_str());
        printf("Error: %s\n", importer.GetErrorString());
    }

    //load_materials(&model, pScene, filename);
    return model;
}

Model load_model(const char* filename) {
    Model model;
    model.pos = {0};
    model.rotate = {0};
    model.scale = {1, 1, 1};

    Assimp::Importer importer;
    const aiScene* pScene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_GenNormals);

    if(pScene) {
        model.meshes.resize(pScene->mNumMeshes);
        model.materials.resize(pScene->mNumMaterials);

        for(u32 i = 0; i < model.meshes.size(); ++i) {
            aiMesh* paiMesh = pScene->mMeshes[i];
            load_mesh(&model, i, paiMesh);
        }
    }
    else {
        printf("Error loading model\n");
    }

    load_materials(&model, pScene, filename);
    return model;
}

void draw_mesh(Mesh mesh) {
    //bind VERTEX ARRAY OBJECT
    //and all attributes of it
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh.vertices.size(), &mesh.vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * mesh.indices.size(), &mesh.indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)0);                     //position
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(3 * sizeof(GLfloat))); //normals
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(6 * sizeof(GLfloat))); //tex coords
	
    glEnableVertexAttribArray(0); //0 = Position
    glEnableVertexAttribArray(1); //1 = Texture Coordinates
    glEnableVertexAttribArray(2); //2 = Normals

    //draw bound VAO using triangles, up to mesh.indexcount indices
    glDrawElements(GL_TRIANGLES, mesh.indexcount, GL_UNSIGNED_SHORT, 0);
}

void draw_model(Model* model) {
        //ONE MATERIAL PER MESH -- DRAW ALL MESHES WITH THEIR MATERIALS (NO TEXTURES IN THESE LOW POLY MODELS, ONLY DIFFUSE COLOR)
        for(const Mesh& mesh : model->meshes) {
            draw_mesh(mesh);
        }
}

void draw_billboard_unordered(Mesh* mesh) {
    //bind attributes and VAO  
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)0);                     //position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(3 * sizeof(GLfloat))); //normals
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(5 * sizeof(GLfloat))); //tex coords
    glEnableVertexAttribArray(0); //0 = Position

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

Mesh create_billboard() {
    std::vector<Vertex>   vertices;
    std::vector<GLushort> indices;

    vec3 normal = {0, 1, 0};
    vertices.push_back({ {-0.5f, -0.5f, 0.0f}, normal, {0, 0} });
    vertices.push_back({ {0.5f, -0.5f, 0.0f},  normal, {0, 1} });
    vertices.push_back({ {-0.5f, 0.5f, 0.0f},  normal, {1, 1} });
    vertices.push_back({ {0.5f, 0.5f, 0.0f},   normal, {1, 0} });

    return create_mesh(vertices, indices);
}

mat4 billboard_transform(f32 x, f32 y, f32 z, vec3 scaleVec, mat4& view) {
	mat4 mat = identity();
	mat *= translation(x, y, z);

    //transpose rotation component of model matrix with view matrix
    mat.m00 = view.m00;
    mat.m02 = view.m20;
    mat.m10 = view.m01;
    mat.m12 = view.m21;
    mat.m20 = view.m02;
    mat.m22 = view.m22;
    //transpose scale component
    mat.m11 = view.m11;
    mat.m33 = view.m33;

    //mat *= rotation(rot, 0, 0, 1);
    mat *= scale(scaleVec.x, scaleVec.y, scaleVec.z);

	return mat;
}
