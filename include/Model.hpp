#ifndef _MODEL
#define _MODEL
#include <vector>
#include <glm/glm.hpp>
// #include <glad/glad.h>
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
class Model
{
private:
    /* data */
    
public:
    unsigned int vertexBuffer;
    unsigned int VAO;

    unsigned int normalBuffer;
    unsigned int uvBuffer;
    std::vector< glm::vec3 > vertex;
    std::vector< glm::vec2 > texture;
    std::vector< glm::vec3 > norm;

    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;

    std::vector<unsigned int> temp_vertex_ind;
    std::vector<unsigned int> temp_uv_ind;
    std::vector<unsigned int> temp_normal_ind;
    Model(const char * path);
    ~Model();
    void draw(GLenum  type);
    void use();
    int get_vertex_num() { return vertex.size(); }
    void export_mesh(const char * path);
};
#endif