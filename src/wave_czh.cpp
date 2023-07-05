#include "wave_czh.hpp"



Wave:: Wave(int d) : d(d) {
        float delta = 1.0f / d;
        for(int i = 0;i < d ; i++) {
            for(int j = 0;j < d;j++) {
                vertex.push_back(glm::vec3(i * delta, j * delta, 0));
                // vertex.push_back(glm::vec3(i * delta, (j+1) * delta, 0));
                // vertex.push_back(glm::vec3((i + 1) * delta, j * delta, 0));
                // vertex.push_back(glm::vec3((i + 1) * delta, (j+1) * delta, 0));
            }
        }
        glGenBuffers(1,&vertexBuffer);
        
        glGenVertexArrays(1,&VAO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(glm::vec3), &vertex[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0 , (void*)0);
}
Wave::~Wave() {

}

void Wave::draw() {
    glDrawArrays(GL_POINTS, 0, vertex.size());
}