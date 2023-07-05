#ifndef _TEXTURE_

#define _TEXTURE_

#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <stb_image.h>

class Texture
{
private:
    /* data */
    // static std::map<std::string,unsigned int> text_manager;
    static std::vector<int> free_texture_stk;
    int idx;
    unsigned int texture;
public:
    Texture(std::string texture_path);
    void update_texture(std::string texture_path);
    int GetSamplerId();
    ~Texture();
};



#endif