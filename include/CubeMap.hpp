#ifndef CUBE_MAP
#define CUBE_MAP

#include <GL/glew.h>
// #include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>
#include <iostream>
#include <vector>

class CubeMap {
    public:
    CubeMap();
    void load(std::vector<std::string> faces);
    unsigned int textureID;
};


#endif