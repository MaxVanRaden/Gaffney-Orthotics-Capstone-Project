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
    mesh->selected_indices.clear();
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
    std::vector<bool> selected; //shadows vertices indicating selected or not
    std::vector<u32> selected_indices; //contains only selected vertices' indices

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
        selected_indices.push_back(i);
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
    model->meshes[i].selected_indices = selected_indices;
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

// function does not yet fully work
// currently being taken care of in main.cpp
Model load_STL (const char* buffer) {
    Model model;
    model.pos = {0};
    model.rotate = {0};
    model.scale = {1, 1, 1};

    Assimp::Importer importer;

    const char *offset = buffer;
    char name[80];
    float temp;

    memcpy(name, offset, 80);//Record file name
    offset += 80;
    int numTriangles;
    memcpy(&numTriangles, offset, 4);//Record the number of triangles
    offset += 4;

    std::string modelData;
    modelData.append("solid OrthoFreeD STLWriter\n");
    for (int i = 0; i < numTriangles; i++) {
        memcpy(&temp, offset, 4);     float normalI = temp;
        memcpy(&temp, offset +4, 4);  float normalJ = temp;
        memcpy(&temp, offset +8, 4);  float normalK = temp;
        std::string line1 =
                "facet normal " +
                std::to_string(normalI) + " " +
                std::to_string(normalJ) + " " +
                std::to_string(normalK) +
                "\n  outer loop\n";
        modelData.append(line1);
        offset += 12;
        for (int j = 0; j < 3; j++) {
            memcpy(&temp, offset, 4);     float verticesX = temp;
            memcpy(&temp, offset +4, 4);  float verticesY = temp;
            memcpy(&temp, offset +8, 4);  float verticesZ = temp;
            std::string line2 =
                    "    vertex " +
                    std::to_string(verticesX) + " " +
                    std::to_string(verticesY) + " " +
                    std::to_string(verticesZ) + "\n";
            modelData.append(line2);
            offset += 12;
        }
        modelData.append("  endloop\nendfacet\n");
        offset += 2;
    }
    modelData.append("endsolid OrthoFreeD STLWriter\n");

    printf("size of transfer: %ld from render.cpp\n", modelData.length());

    for(int i = 0; i < modelData.length() +1; i++){
        printf("%c", modelData[i]);
    }


    const aiScene *pScene = importer.ReadFileFromMemory(
            (void *) &modelData[0], modelData.size(),
            aiProcess_FlipUVs         |
            aiProcess_GenSmoothNormals      |
            aiProcess_Triangulate           |
            aiProcess_FindInvalidData       |
            aiProcess_ValidateDataStructure |
            0, ".stl");
    if(!pScene) {
        printf("%s failed to load, (string)\n", modelData.c_str());
    } else {
        model.meshes.resize(pScene->mNumMeshes);
        model.materials.resize(pScene->mNumMaterials);

        for (u32 i = 0; i < pScene->mNumMeshes; ++i) {
            aiMesh *paiMesh = pScene->mMeshes[i];
            load_mesh(&model, i, paiMesh);
        }
    }

    return model;

}

#include <assimp/cimport.h>
Model load_model_string(const std::string& filepath, int fileformat) {
    Model model;
    model.pos = {0};
    model.rotate = {0};
    model.scale = {1, 1, 1};

    Assimp::Importer importer;

    std::string pHint;
    std::string newpath;
    // fileformat 0: obj
    //            1: stl (ascii)
    //            2: stl (binary)
    //            3: memfs filepath
    //            4: null
    // I'll switch the fileformat order around later making 0 == null
    // no change for now to maintain branch compatibility

    if (fileformat == 3) {
        const aiScene *pScene = importer.ReadFile( filepath,
               aiProcess_FlipUVs        |
               aiProcess_GenSmoothNormals      |
               aiProcess_Triangulate           |
               aiProcess_FindInvalidData       |
               aiProcess_ValidateDataStructure);
        if(!pScene) {
            printf("%s failed to load, (file)\n", filepath.c_str());
        } else {
            model.meshes.resize(pScene->mNumMeshes);
            model.materials.resize(pScene->mNumMaterials);

            for (u32 i = 0; i < pScene->mNumMeshes; ++i) {
                aiMesh *paiMesh = pScene->mMeshes[i];
                load_mesh(&model, i, paiMesh);
            }
        }
        return model;

    } else if (fileformat == 2){

        return load_STL(&filepath[0]);

    } else if (fileformat == 0) {
        pHint.append(".obj");
        printf("obj format processing...\n");
    } else if (fileformat == 1) {
        pHint.append(".stl");
        printf("stl ascii format processing...\n");
    }
    const aiScene *pScene = importer.ReadFileFromMemory(
            (void *) &filepath[0], filepath.size(),
            aiProcess_FlipUVs         |
            aiProcess_GenSmoothNormals      |
            aiProcess_Triangulate           |
            aiProcess_FindInvalidData       |
            aiProcess_ValidateDataStructure |
            0, pHint.c_str());
    if(!pScene) {
        printf("%s failed to load, (string)\n", filepath.c_str());
    } else {
        model.meshes.resize(pScene->mNumMeshes);
        model.materials.resize(pScene->mNumMaterials);

        for (u32 i = 0; i < pScene->mNumMeshes; ++i) {
            aiMesh *paiMesh = pScene->mMeshes[i];
            load_mesh(&model, i, paiMesh);
        }
    }
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

mat4 no_view_scaling_transform(f32 x, f32 y, f32 z, vec3 scaleVec, mat4& view) {
    mat4 mat = identity();
    mat *= translation(x, y, z);

    //transpose scale component
    mat.m00 = view.m00;
    mat.m22 = view.m22;
    mat.m11 = view.m11;
    mat.m33 = view.m33;

    //mat *= rotation(rot, 0, 0, 1);
    mat *= scale(scaleVec.x, scaleVec.y, scaleVec.z);

    return mat;
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
