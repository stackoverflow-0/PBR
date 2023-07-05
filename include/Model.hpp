#pragma once

#include <vector>
#include <glm/glm.hpp>
// #include <glad/glad.h>
#include <GL/glew.h>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

class Model
{
private:
    /* data */
    char * path {nullptr};
public:
    // unsigned int vertexBuffer;
    // unsigned int VAO;

    // unsigned int normalBuffer;
    // unsigned int uvBuffer;
    // std::vector< glm::vec3 > vertex;
    // std::vector< glm::vec2 > texture;
    // std::vector< glm::vec3 > norm;

    // std::vector< glm::vec3 > temp_vertices;
    // std::vector< glm::vec2 > temp_uvs;
    // std::vector< glm::vec3 > temp_normals;

    // std::vector<unsigned int> temp_vertex_ind;
    // std::vector<unsigned int> temp_uv_ind;
    // std::vector<unsigned int> temp_normal_ind;
    Model(char * path) : path(path) {};
    void loadMesh();
    void processMesh(aiNode * node, const aiScene * scene);
    ~Model() {};
    void draw(GLenum  type);
    // void use();
    // int get_vertex_num() { return vertex.size(); }
};
