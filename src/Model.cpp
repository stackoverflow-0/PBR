#include "Model.hpp"

// Model::Model(const char * path) {
//     std::ifstream objfile(path);

//     std::string info;
//     std::getline(objfile,info);
//     std::getline(objfile,info);
//     std::getline(objfile,info);
//     std::string flag;

//     // std::vector< glm::vec3 > temp_vertices;
//     // std::vector< glm::vec2 > temp_uvs;
//     // std::vector< glm::vec3 > temp_normals;

//     // std::vector<unsigned int> temp_vertex_ind;
//     // std::vector<unsigned int> temp_uv_ind;
//     // std::vector<unsigned int> temp_normal_ind;
//     while (true) {
//         objfile >> flag;

//         if(flag == "v") {
//             glm::vec3 vec ;
//             objfile >> vec.x >> vec.y >> vec.z;
//             temp_vertices.push_back(vec);
//         }
//         else if(flag == "vt") {
//             glm::vec2 vec ;
//             objfile >> vec.x >> vec.y;
//             temp_uvs.push_back(vec);
//         }
//         else if(flag == "vn") {
//             glm::vec3 vec;
//             objfile >> vec.x >> vec.y >> vec.z;
//             temp_normals.push_back(vec);
//         }
//         else if (flag == "f") {
//             unsigned int vertex_ind , texture_ind , norm_ind;
//             char t;
//             for (int i = 0;i < 3;i ++) {
//                 objfile >> vertex_ind >> t >> texture_ind >> t >> norm_ind;
//                 temp_vertex_ind.push_back(vertex_ind-1);
//                 temp_uv_ind.push_back(texture_ind-1);
//                 temp_normal_ind.push_back(norm_ind-1);
//             }
//         }
//         if (objfile.eof()) break;
//     }
//     objfile.close();
//     for(int i = 0;i < temp_vertex_ind.size();i++) {
//         vertex.push_back(temp_vertices[temp_vertex_ind[i]]);
//     }
//     for(int i = 0;i < temp_uv_ind.size();i++) {
//         texture.push_back(temp_uvs[temp_uv_ind[i]]);
//     }
//     for(int i = 0;i < temp_normal_ind.size();i++) {
//         norm.push_back(temp_normals[temp_normal_ind[i]]);
//     }

//     glGenBuffers(1,&vertexBuffer);
//     glGenBuffers(1,&normalBuffer);
//     glGenBuffers(1,&uvBuffer);

//     glGenVertexArrays(1,&VAO);
//     glBindVertexArray(VAO);

//     glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
//     glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(glm::vec3), &vertex[0], GL_STATIC_DRAW);

//     glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
//     glBufferData(GL_ARRAY_BUFFER, norm.size() * sizeof(glm::vec3), &norm[0], GL_STATIC_DRAW);

//     glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
//     glBufferData(GL_ARRAY_BUFFER, texture.size() * sizeof(glm::vec2), &texture[0], GL_STATIC_DRAW);

//     glEnableVertexAttribArray(0);
//     glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float) , (void*)0);

//     glEnableVertexAttribArray(1);
//     glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
//     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float) , (void*)0);

//     glEnableVertexAttribArray(2);
//     glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
//     glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float) , (void*)0);

// }

// Model::~Model() {
//     glDeleteBuffers(1,&vertexBuffer);
//     glDeleteBuffers(1,&normalBuffer);
//     glDeleteBuffers(1,&uvBuffer);
//     glDeleteVertexArrays(1,&VAO);
// }

// void Model::draw(GLenum  type) {
//     use();
//     glDrawArrays(type,0,vertex.size());
// }

// void Model::use() {
//     glBindVertexArray(VAO);
// }

void Model::loadMesh()
{
    assert(path != nullptr);
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cout << "error assimp : " << importer.GetErrorString() << endl;
    }
}

void Model::processMesh(aiNode *node, const aiScene *scene)
{
    for (int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
        
    }
}
