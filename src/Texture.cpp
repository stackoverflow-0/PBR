#include "Texture.hpp"

Texture::Texture(std::string texture_path)
{
    // if (text_manager.find(texture_path) == text_manager.end()) {
    //     std::cout<<"texture name already exist"<<std::endl;
    // }
    
    // text_manager[texture_path] = 0;
    glGenTextures(1,&texture);
    idx = free_texture_stk.back();
    glActiveTexture(GL_TEXTURE0 + idx);
    free_texture_stk.pop_back();

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *img = stbi_load(texture_path.c_str(), &width, &height, &nrChannels, 0);
    if (img) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    // std::cout<<"h"<<height<<"w"<<width<<"channel"<<nrChannels<<std::endl;
    stbi_image_free(img);

}

void Texture::update_texture(std::string texture_path){
    glActiveTexture(GL_TEXTURE0 + idx);
    glBindTexture(GL_TEXTURE_2D, texture);
    int width, height, nrChannels;
    unsigned char *img = stbi_load(texture_path.c_str(), &width, &height, &nrChannels, 0);
    if (img) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, img);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    // std::cout<<"h"<<height<<"w"<<width<<"channel"<<nrChannels<<std::endl;
    stbi_image_free(img);

}


Texture::~Texture()
{
    free_texture_stk.push_back(idx);
}

std::vector<int> Texture::free_texture_stk = {7,6,5,4,3,2};
// std::vector<int> Texture::free_texture_stk = {7,6,5};

int Texture::GetSamplerId() { return idx; }