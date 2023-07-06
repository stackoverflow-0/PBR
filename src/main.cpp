#include <GL/glew.h>
// #include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/constants.hpp>

#include "shader.h"
#include "Camera.h"

#include <iostream>
#include <math.h>
#include <time.h>
#include <string>
#include <sstream>

#include "Model.hpp"
#include "Texture.hpp"

#include <stb_image.h>

int SCR_WIDTH = 800, SCR_HEIGHT = 600;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);



float deltaTime = 0.0f;
float lastFrame = 0.0f;

void frame_size_callback(GLFWwindow *window, int w, int h)
{
    glViewport(0, 0, w, h);
}

float lastX = 0.0, lastY = 0.0;
float yaw = 0.0, pitch = 0.0;
bool firstMouse = true;

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{

    // if(firstMouse) {
    //     lastX = xpos;
    //     lastY = ypos;
    //     firstMouse = false;
    // }
    if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        // lastX = xpos;
        // lastY = ypos;
        return;
    }

    // float xoffset = xpos - lastX ;
    // float yoffset = lastY - ypos ;
    float xoffset = xpos - SCR_WIDTH / 2;
    float yoffset = SCR_HEIGHT / 2 - ypos;
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    glm::vec3 direction;
    direction.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = -cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);

    glfwSetCursorPos(window, SCR_WIDTH / 2, SCR_HEIGHT / 2);
    return;
}

unsigned int draw_mode = GL_TRIANGLES;

void input_process(GLFWwindow *window)
{
    static float cd = 0;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && cd >= 0.1f)
    {
        draw_mode = (draw_mode == GL_TRIANGLES) ? GL_LINE_LOOP : GL_TRIANGLES;
        cd = 0.0f;
    }
    cd += deltaTime;
    float tv = 0.5f * deltaTime;
    float cameraSpeed = 2.0f * deltaTime; // adjust accordingly

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
        
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraUp;
}

GLFWwindow *init_window()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "PBR", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glfwSetFramebufferSizeCallback(window, frame_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetCursorPos(window, SCR_WIDTH / 2, SCR_HEIGHT / 2);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");
    glEnable(GL_DEPTH_TEST);
    // 背面剔除
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    return window;
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    return textureID;
}

int main()
{
    auto window = init_window();
    /*
        ------------------------------------------------------------------------
        Now u dont need to call shader.use before u set value to uniform virable
        ------------------------------------------------------------------------
    */
    auto pbrShader = Shader("./shaders/PBR.vs", "./shaders/PBR.fs");
    std::cout << "pbr complie success\n";
    auto depthShader = Shader("./shaders/depth_vshader.vs", "./shaders/depth_fshader.fs");
    std::cout << "depth complie success\n";
    // auto debugDepthQuad = Shader("./shaders/debug_depth_vshader.vs","./shaders/debug_depth_fshader.fs");
    Model object("asset/beowulf-the-low-poly-book/source/beowulf.obj");


    // glm::vec3 objectColor = glm::vec3(0.8, 0.8, 0.8);

    glm::vec3 lightColor = glm::vec3(1.0, 1.0, 1.0);

    glm::vec3 lightPos = glm::vec3(3.0, 5.0f, 3.0);

    pbrShader.setVec3("lightColor", lightColor);
    pbrShader.setVec3("lightPos", lightPos);

    // glm::mat4 view;

    // const float radius = 10.0f;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // about depth texture
    unsigned int depthMapFBO;
    unsigned int depthMap;
    const unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
    {
        glGenFramebuffers(1, &depthMapFBO);

        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    // load skybox
    std::vector<std::string> faces{
        "asset/skybox/right.jpg",
        "asset/skybox/left.jpg",
        "asset/skybox/top.jpg",
        "asset/skybox/bottom.jpg",
        "asset/skybox/front.jpg",
        "asset/skybox/back.jpg"};

    unsigned int skyboxTexture = loadCubemap(faces);
    pbrShader.setInt("skybox", 0);

    auto skybox = Model("asset/skybox/HD_sphere.obj");
    auto skybox_shader = Shader("./shaders/skybox_vs.vs", "./shaders/skybox_fs.fs");

    auto albedoMap = Texture("asset/beowulf-the-low-poly-book/textures/beowulf_color.jpg");
    auto metallicMap = Texture("asset/beowulf-the-low-poly-book/textures/beowulf_metal.jpg");
    auto roughnessMap = Texture("asset/beowulf-the-low-poly-book/textures/beowulf_rough.jpg");
    auto aoMap = Texture("asset/pbr-rivet-gun/textures/internal_ground_ao_texture.png");
    auto normalMap = Texture("asset/beowulf-the-low-poly-book/textures/beowulf_bump.jpg");
    
    pbrShader.setInt("albedoMap", albedoMap.GetSamplerId());
    pbrShader.setInt("metallicMap", metallicMap.GetSamplerId());
    pbrShader.setInt("roughnessMap", roughnessMap.GetSamplerId());
    pbrShader.setInt("aoMap", aoMap.GetSamplerId());
    pbrShader.setInt("normalMap", normalMap.GetSamplerId());
    
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        // std::cout<<deltaTime*60<<std::endl;
        lastFrame = currentFrame;
        // pbrShader.setFloat("t", currentFrame);
        input_process(window);

        glm::mat4 view;
        glm::mat4 projection;

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        projection = glm::perspective(glm::radians(45.0f), SCR_WIDTH * 1.0f / SCR_HEIGHT, 0.1f, 100.0f);
        pbrShader.setVec3("viewPos", cameraPos);

        auto model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.4, 0.4, 0.4));
        // model = glm::rotate(model, currentFrame , glm::vec3(0.0f,1.0f,1.0f));
        glm::mat4 model_inv_T = glm::transpose(glm::inverse(model));

        /*
            -----------
            point light
            -----------
        */
        float near_plane = 0.0f, far_plane = 10.0f;
        float s = 2.0f;
        glm::mat4 lightProjection = glm::ortho(-s, s, -s, s, near_plane, far_plane);
        // auto dir =  glm::vec4(0,0,1,1);
        glm::mat4 lightView = glm::lookAt(
            lightPos, // lightPos,
            glm::vec3(0, 0, 0),
            glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        // draw sky box

        // depthShader.setFloat("t", currentFrame);
        depthShader.setMat4("lightSpaceMatrix", glm::value_ptr(lightSpaceMatrix));
        // depthShader.setMat4("model",glm::value_ptr(glm::mat4(1.0f)));
        depthShader.setMat4("model", glm::value_ptr(model));
        /*
            ----------------------------
            draw shadow with framebuffer
            ----------------------------
        */

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glDisable(GL_CULL_FACE);
        glClear(GL_DEPTH_BUFFER_BIT);

            depthShader.use();
            object.draw(GL_TRIANGLES);

        glEnable(GL_CULL_FACE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // 2. then render scene as normal with shadow mapping (using depth map)
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);

        glDisable(GL_CULL_FACE);

        skybox_shader.setMat4("projection", glm::value_ptr(projection));
        skybox_shader.setMat4("view", glm::value_ptr(view));
        skybox_shader.setInt("skybox", 0);

        skybox_shader.use();
        skybox.draw(GL_TRIANGLES);

        // glEnable(GL_CULL_FACE);
        pbrShader.use();
        pbrShader.setInt("shadowMap", 1);
        pbrShader.setMat4("view", glm::value_ptr(view));
        pbrShader.setMat4("projection", glm::value_ptr(projection));
        pbrShader.setMat4("lightSpaceMatrix", glm::value_ptr(lightSpaceMatrix));
        pbrShader.setMat4("model", glm::value_ptr(model));
        pbrShader.setMat4("model_inv_T", glm::value_ptr(model_inv_T));
        

        object.draw(GL_TRIANGLES);


        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}