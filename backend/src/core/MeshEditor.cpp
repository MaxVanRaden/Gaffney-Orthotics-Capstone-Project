#include <iostream>
#include <fstream>
#include "MeshEditor.h"
#include "StairsString.h"
#include "assimp/Exporter.hpp"

MeshEditor::MeshEditor() {
    export_strlen = 0;
    shader.load();
    bshader.load();
    camera = {0};
    entities.emplace_back(staircaseobjhardcoded);

//    export_new(1, "obj");
    entities.emplace_back();
    entities.back().load(staircaseobjhardcoded);
    entities.back().set_position( {4, 4, 4} );
    //entities.emplace_back();
    //entities.back().load(staircaseobjhardcoded);
    //entities.back().set_position( {-3, -3, -3} );
    //entities.emplace_back();
    //entities.back().load(staircaseobjhardcoded);
    //entities.back().set_position( {-5, -5, -5} );
    //entities.emplace_back();
    //entities.back().load(staircaseobjhardcoded);
    //entities.back().set_position( {3, -3, -5} );
    //entities.emplace_back();
    //entities.back().load(staircaseobjhardcoded);
    //entities.back().set_position( {-3, -3, 3} );
    projection = perspective_projection(90, 16.0f / 9.0f, 0.01f, 3000.0f);
    move_cam_backwards(&camera, 10);

    billboard = create_billboard();

    //Create a circle texture to be used to show where vertices are on the mesh
    auto* pixels = new unsigned char[64*64*4];
    vec2 center = {32.0f, 32.0f};
    for(int x = 0; x < 64; ++x) {
        for(int y = 0; y < 64; ++y) {
            vec2 pos = {(float)x, (float)y};
            int index = (x+y*64)*4;

            vec2 diff = center - pos;
            f32 distance = length(diff);

            if(distance < 32) {
                pixels[index+0] = 255;
                pixels[index+1] = 255;
                pixels[index+2] = 255;
                pixels[index+3] = 255;
            } else {
                pixels[index+0] = 0;
                pixels[index+1] = 0;
                pixels[index+2] = 0;
                pixels[index+3] = 0;
            }
        }
    }
    circle = load_texture(pixels, 64, 64, GL_LINEAR);
    delete[] pixels;
}

void MeshEditor::run() {
    local float rotation = 0.0f;
    rotation+=0.2f;
    local float zoom = 5.0f;
    //zoom-=0.025f;

#if 0
    Rect viewport = {0, 0, 1000, 640};
    vec2 mouse;
    int x;
    int y;
    glfwGetMousePos(&x, &y);
    mouse.x = x;
    mouse.y = y;

    vec3 rayposition = {camera.x, camera.y, camera.z};
    vec3 raydirection = raycast(projection, camera, mouse, viewport);

    for(int i = 0; i < entities.size(); ++i) {
        if(entities[i].is_mouse_over(rayposition, raydirection)) {
            printf("mouse is over a mesh\n");
        } else {
            printf("mouse is not over a mesh\n");
        }
    }
#endif

    mat4 view = create_view_matrix(camera);

    shader.bind();
    shader.set_light_pos(14, 14, 14);
    shader.set_light_color(147.0f/255.0f, 108.0f/255.0f,95.0f/255.0f);
    shader.set_camera_pos(camera.x, camera.y, camera.z);
    shader.set_view(view);

    static int timer=0;
    timer++;

    for(Entity& e : entities) {
        e.draw(shader);
        e.set_rotation( {rotation, rotation, rotation} );
    }
    bshader.bind();
    bshader.set_view(view);
    for(Entity& e : entities) {
        e.draw_vertices(bshader, &billboard, circle, view, {camera.x, camera.y, camera.z});
    }
}

void MeshEditor::set_camera(float zoom, float x, float y, float z, float yaw, float pitch, float roll){
    camera = {x, y, z, pitch, yaw, roll};
}

void MeshEditor::add_model(const char* str) {
    entities.emplace_back();
    entities.back().load(str);
    printf("added model\n");
}

char* MeshEditor::export_model(int ID, const char* fileformat) {
    int number = 0;
    Assimp::Exporter exporter;
    aiScene *scene = new aiScene();
    int count = 0;
    for(Entity e : entities) {
        count += e.get_current().meshes.size();
    }
    scene->mRootNode = new aiNode();
    scene->mMeshes = new aiMesh*[count];
    scene->mNumMeshes = count;
    scene->mNumTextures = 0;
    scene->mNumAnimations = 0;
    scene->mNumMaterials = 0;
    scene->mNumCameras = 0;

    int i = 0;
    for(Entity e : entities) {
        Model curr = e.get_current();
        for(Mesh m : curr.meshes) {
            scene->mMeshes[i] = new aiMesh();
            scene->mMeshes[i]->mNumBones = 0;
            scene->mMeshes[i]->mNumAnimMeshes = 0;
            scene->mMeshes[i]->mNumVertices = m.vertices.size();
            scene->mMeshes[i]->mNumFaces = m.indices.size() / 3;
            scene->mMeshes[i]->mVertices = new aiVector3t<ai_real>[m.vertices.size()];
            scene->mMeshes[i]->mNormals = new aiVector3t<ai_real>[m.vertices.size()];
            scene->mMeshes[i]->mFaces = new aiFace[m.indices.size() / 3];
            int j = 0;
            for(Vertex v : m.vertices) {
                scene->mMeshes[i]->mVertices[j] = aiVector3t<ai_real>(v.position.x, v.position.y, v.position.z);
                j++;
            }
            j = 0;
            for(Vertex v : m.vertices) {
                scene->mMeshes[i]->mNormals[j] = aiVector3t<ai_real>(v.normal.x, v.normal.y, v.normal.z);
                j++;
            }
            for(j = 0; j < m.indices.size(); j+=3) {
                scene->mMeshes[i]->mFaces[j/3].mNumIndices = 3;
                scene->mMeshes[i]->mFaces[j/3].mIndices = new unsigned int[3];
                scene->mMeshes[i]->mFaces[j/3].mIndices[0] = m.indices[j+0];
                scene->mMeshes[i]->mFaces[j/3].mIndices[1] = m.indices[j+1];
                scene->mMeshes[i]->mFaces[j/3].mIndices[2] = m.indices[j+2];
            }
            i++;
        }
    }

    std::cout << "got to " << ++number << std::endl;
    scene->mRootNode->mName = "ExportTest";
    std::cout << "got to " << ++number << std::endl;
    scene->mRootNode->mChildren = NULL;
    std::cout << "got to " << ++number << std::endl;
    scene->mRootNode->mNumChildren = 0;
    std::cout << "got to " << ++number << std::endl;
    scene->mRootNode->mNumMeshes = count;
    std::cout << "got to " << ++number << std::endl;
    scene->mRootNode->mMeshes = new unsigned int[count];
    std::cout << "got to " << ++number << std::endl;
    for(int k = 0; k < count; ++k) {
        scene->mRootNode->mMeshes[k] = k;
    }

    std::cout << "got to " << ++number << std::endl;
    printf("test: %p\n", (void*)scene);
//    const aiExportDataBlob* blob = exporter.ExportToBlob(scene, "obj", 0);
    size_t my_size = exporter.GetExportFormatCount();
    printf("my_size: %d\n", my_size);
    for(int i = 0; i < my_size; ++i) {
        const aiExportFormatDesc* formatDesc = exporter.GetExportFormatDescription(i);
        if (!formatDesc){
            printf("i: %d is null\n", i);
        } else {
            printf("i: %d, id: %s, desc: %s, ext: %s\n", i, formatDesc->id, formatDesc->description, formatDesc->fileExtension);
        }
    }

    exporter.Export(scene, "objnomtl", "testfile");
    std::cout << "got to " << ++number << std::endl;
//    printf("blob: %s\n", (char*)blob->data);
    return nullptr;
//    return (char*)blob->data;
}

void MeshEditor::on_mouse_up(int x, int y, int x2, int y2) {
    for(Entity& e: entities) {
        e.select(x, y, x2, y2, camera, projection, {0, 0, 1000, 640});
    }
}

// Scale every vertex in every mesh in every entity by the factor passed in
void MeshEditor::scale_all_entities(float factor) {
    for(Entity& e: entities) {
        e.scale_entity(factor);
    }
}

char* MeshEditor::export_new(int ID, const char *fileformat) {
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
//            for (Vertex v: m.vertices) {
//                float texturesX = m.vertices[j].uv.x;
//                float texturesY = m.vertices[j].uv.y;
//                std::string line = "vt " + std::to_string(texturesX) + " " + std::to_string(texturesY) + "\n";
//                modelData.append(line);
//                j++;
//            }
//            j = 0;
//
//            //vertex normals
//            for (Vertex v: m.vertices) {
//                float normalsX = m.vertices[j].normal.x;
//                float normalsY = m.vertices[j].normal.y;
//                float normalsZ = m.vertices[j].normal.z;
//                std::string line = "vn " + std::to_string(normalsX) + " " + std::to_string(normalsY) + " " + std::to_string(normalsZ) + "\n";
//                modelData.append(line);
//                j++;
//            }
//
            //now the faces, which needs work
            for (j = 0; j < m.indices.size(); j+=3) {
                int face1 = m.indices[j] + 1;
                int face2 = m.indices[j + 1] + 1;
                int face3 = m.indices[j + 2] + 1;
//                std::string line =
//                        "f " + std::to_string(face1) + "//" + std::to_string(face1) +
//                        " " + std::to_string(face2) +  "//" + std::to_string(face2) +
//                        " " + std::to_string(face3) + "//" + std::to_string(face3) + "\n";
                std::string line = "f " + std::to_string(face1) + " " + std::to_string(face2) + " " + std::to_string(face3) + "\n";
                modelData.append(line);
            }
//            for (int k = 0; k < m.indices.size(); ++k){
//                std::cout << m.indices[k] << std::endl;
//            }
        }
    }

    // Return the string containing the vertices and faces of the model
    char * temp = new char[modelData.size()];
    strcpy(temp, modelData.c_str());

    // Set the export strlen
    export_strlen = modelData.size();
    return temp;
}

// Dumby function to return the size (in bytes) of the export string
// Will return 0 if export function has not been called yet.
// Seems dumb, but necessary because we have to pass to front-end in a weird way.
// Talk to Jacob if you feel like you have a better solution and I'll hear you out.
uint32_t MeshEditor::get_export_strlen() {
    return export_strlen;
}

MeshEditor::~MeshEditor() {
    shader.dispose();
    bshader.dispose();
}