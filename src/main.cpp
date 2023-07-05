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

glm::vec3 cameraPos = glm::vec3(0.0f, 8.0f, 10.0f);
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
    // if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
    //     lastX = xpos;
    //     lastY = ypos;
    //     return;
    // }

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
    auto shader = Shader("./shaders/t_sphere_vshader.vs", "./shaders/t_sphere_fshader.fs", "./shaders/t_sphere_gshader.gs");
    auto depthShader = Shader("./shaders/depth_vshader.vs", "./shaders/depth_fshader.fs");
    // auto debugDepthQuad = Shader("./shaders/debug_depth_vshader.vs","./shaders/debug_depth_fshader.fs");

    // Model sea("asset/model/NM_sphere.obj");
    // Model boat("asset/model/boat.obj");
    // boat.export_mesh("asset/export/boat.obj");
    // Model lighthouse("asset/model/lighthouse.obj");

    glm::vec3 objectColor = glm::vec3(0.8, 0.8, 0.8);

    glm::vec3 lightColor = glm::vec3(1.0, 1.0, 1.0);

    glm::vec3 lightPos = glm::vec3(0.0, 10.0f, 0.0);

    shader.setVec3("objectColor", objectColor);
    shader.setVec3("lightColor", lightColor);
    shader.setVec3("lightPos", lightPos);

    shader.setVec3("albedo", glm::vec3(0.0f, 0.0f, 1.0f));
    shader.setFloat("ao", 1.0);
    shader.setFloat("metallic", 0.9);
    shader.setFloat("roughness", 0.1);

    // glm::mat4 view;

    const float radius = 10.0f;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // about depth texture
    unsigned int depthMapFBO;
    unsigned int depthMap;
    const unsigned int SHADOW_WIDTH = 4096 * 2, SHADOW_HEIGHT = 4096 * 2;
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
    shader.setInt("skybox", 0);

    auto skybox = Model("asset/HD_sphere.obj");
    auto skybox_shader = Shader("./shaders/skybox_vs.vs", "./shaders/skybox_fs.fs");
    auto particleshader = Shader("./shaders/particle.vs", "./shaders/particle.fs", "./shaders/particle.gs");

    // auto suntexture = Texture("./asset/Water_COLOR.jpg");
    // auto boat_texture = Texture("./asset/boat.jpg");
    // // auto water_height = Texture("./asset/Water.jpg");
    // auto water_height = Texture("asset/water_dis/Calm_loop_tile_01001.jpg");
    // auto lighthouse_texture = Texture("./asset/lighthouse.jpg");
    // auto water_height = Texture("asset/water_dis_all.jpg");

    /*
        -----------------------------------------------------------
        Notice ! set default depth texture = 0 , skybox texture = 1
        -----------------------------------------------------------
    */

    // glm::mat4 sea_model_mat = glm::mat4(1.0f);
    // sea_model_mat = glm::scale(sea_model_mat,glm::vec3(5,5,5));

    // int wave_frame_id = 1;
    // char water_dis_path[256];

    auto dir_rotate_mat = glm::mat4(1.0f);
    auto sea_rotate_mat = glm::mat4(1.0f);
    // auto tower_rotate_mat = glm::mat4(1.0f);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        // std::cout<<deltaTime*60<<std::endl;
        lastFrame = currentFrame;
        shader.setFloat("t", currentFrame);

        input_process(window);

        // sprintf(water_dis_path,"asset/water_dis/Calm_loop_tile_01%03d.jpg",wave_frame_id);

        // auto water_height = Texture(water_dis_path);
        // wave_frame_id = wave_frame_id%152 + 1;
        // int slow_down_wave = 1;
        // int slow_down_wave = 5;
        // wave_frame_id = wave_frame_id%(10 * slow_down_wave) + 1;
        // std::cout<<(wave_frame_id-1)/slow_down_wave<<std::endl;
        // cameraPos = glm::vec3(0,8,0);
        glm::mat4 view;
        glm::mat4 projection;

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        projection = glm::perspective(glm::radians(45.0f), SCR_WIDTH * 1.0f / SCR_HEIGHT, 0.1f, 100.0f);
        shader.setVec3("viewPos", cameraPos);

        // glStencilFunc(GL_ALWAYS, 1, 0xFF);
        // glStencilMask(0xFF);

        // get model martix
        // draw_mode = GL_POINTS;

        // boat_left = glm::normalize(glm::cross(boat_pos,boat_v));

        // if(speed<max_speed){
        //     speed+=0.01f;
        // }
        // else speed = max_speed;

        // auto delta_rotate_mat = glm::rotate(glm::mat4(1.0f),delta_angle,glm::vec3(0,1,0));

        // dir_rotate_mat = dir_rotate_mat * delta_rotate_mat ;

        // auto delta_sea_rotate_mat = glm::rotate(
        //     glm::mat4(1.0f),speed* deltaTime,glm::vec3(dir_rotate_mat * glm::vec4(boat_left,1))
        // );
        // sea_rotate_mat = delta_sea_rotate_mat * sea_rotate_mat;
        // auto sea_model_mat = glm::scale(
        //     sea_rotate_mat,
        //     glm::vec3(world_scale_rate,world_scale_rate,world_scale_rate)
        //     // glm::vec3(5.0f,5.0f,5.0f)
        // );
        // auto tower_model_mat = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0,0,1));
        // tower_model_mat = glm::translate(tower_model_mat,glm::vec3(0,world_scale_rate,0));
        // tower_model_mat = glm::scale(
        //     sea_rotate_mat * tower_model_mat,
        //     // glm::vec3(world_scale_rate * 0.1f,world_scale_rate * 0.1f,world_scale_rate * 0.1f)
        //     glm::vec3(0.1f,0.1f,0.1f)
        // );

        // // auto boat_rotate_mat =  glm::translate(glm::mat4(1.0f), glm::vec3(0,5,0));
        // auto boat_rotate_mat = glm::rotate(glm::mat4(1.0f), angle, boat_up);
        // // boat_rotate_mat = boat_shake_mat * boat_rotate_mat;
        // // std::cout<<"angle : "<<angle<<std::endl;
        // auto boat_model_mat = glm::scale(
        //     glm::translate(boat_rotate_mat,boat_pos),
        //     glm::vec3(0.5,0.5,0.5)
        // );
        // boat_model_mat = glm::rotate(boat_model_mat,0.3f * glm::sin(currentFrame),glm::vec3(0,0,1));

        // angle = 0;

        // coliision detect
        // auto world_boat_head = glm::vec3(boat_model_mat*glm::vec4(boat_init_v,1));
        // auto world_boat_left = glm::vec3(boat_model_mat*glm::vec4(boat_left,1));
        // auto world_boat_base = glm::vec3(boat_model_mat*glm::vec4(0,0,0,1));
        // auto boathead2tower = glm::vec3(tower_model_mat*glm::vec4(0,0,0,1)) - world_boat_head;
        // bool collide = glm::length(boathead2tower) < collide_distance;
        // if(collide){
        //     float front_component = glm::dot(glm::normalize(boathead2tower), glm::normalize(world_boat_head - world_boat_base));
        //     float left_component = glm::dot(glm::normalize(boathead2tower), glm::normalize(world_boat_left - world_boat_base));
        //     std::cout<<"delta_angle:"<<delta_angle<<std::endl;
        //     std::cout<<"collide detected! front_comp:"<<front_component<<" left_comp:"<<left_component<<std::endl;
        //     collide_delta_angle = left_component * 0.1 * (collide_distance - glm::length(boathead2tower) + 0.2);
        //     if(speed>0) speed = 1.2*speed*(1-2*front_component*front_component);
        // }
        // delta_angle = 0;

        /*
            -----------
            point light
            -----------
        */
        float near_plane = 0.0f, far_plane = 50.0f;
        float s = 8.0f;
        glm::mat4 lightProjection = glm::ortho(-s, s, -s, s, near_plane, far_plane);
        // auto dir =  glm::vec4(0,0,1,1);
        glm::mat4 lightView = glm::lookAt(
            glm::vec3(sea_rotate_mat * glm::vec4(0, 10.0f, 0, 1)), // lightPos,
            glm::vec3(sea_rotate_mat * glm::vec4(0, 0, 0, 1)),
            glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        // draw sky box

        depthShader.setFloat("t", currentFrame);
        depthShader.setMat4("lightSpaceMatrix", glm::value_ptr(lightSpaceMatrix));
        // depthShader.setMat4("model",glm::value_ptr(glm::mat4(1.0f)));

        /*
            ----------------------------
            draw shadow with framebuffer
            ----------------------------
        */

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glDisable(GL_CULL_FACE);
        glClear(GL_DEPTH_BUFFER_BIT);

        // depthShader.setMat4("model",glm::value_ptr(sea_model_mat));
        // // sea.use();
        // sea.draw(GL_TRIANGLES);
        // depthShader.setMat4("model",glm::value_ptr(sea_model_mat));
        // // depthShader.setBool("is_waving",true);
        // // // depthShader.setBool("use_height_texture",true);
        // // depthShader.setInt("wave_frame_id",(wave_frame_id-1)/slow_down_wave + 1);
        // // depthShader.setInt("wave_height_sampler",water_height.GetSamplerId());
        // sea.draw(GL_TRIANGLES);

        // depthShader.setMat4("model",glm::value_ptr(boat_model_mat));
        // depthShader.setBool("is_waving",false);
        // // depthShader.setBool("use_height_texture",false);
        // boat.draw(GL_TRIANGLES);
        // depthShader.setMat4("model",glm::value_ptr(tower_model_mat));
        // depthShader.setBool("is_waving",false);
        // // depthShader.setBool("use_height_texture",false);
        // lighthouse.draw(GL_TRIANGLES);
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

        // shader.setInt("shadowMap",1);
        // shader.setMat4("view",glm::value_ptr(view));
        // shader.setMat4("projection",glm::value_ptr(projection));
        // shader.setMat4("lightSpaceMatrix",glm::value_ptr(lightSpaceMatrix));

        // shader.setMat4("model",glm::value_ptr(sea_model_mat));

        // // example about using texture
        // shader.setBool("use_sampler",false);
        // shader.setBool("is_waving",true);
        // shader.setInt("texture_sampler",suntexture.GetSamplerId());
        // shader.setInt("wave_frame_id",(wave_frame_id-1)/slow_down_wave);
        // shader.setInt("wave_height_sampler",water_height.GetSamplerId());

        // // it is recommanded to call shader.use before model.draw

        // shader.use();
        // sea.draw(GL_TRIANGLES);

        // shader.setMat4("model",glm::value_ptr(boat_model_mat));
        // shader.setVec3("objectColor",glm::vec3(1.0,0,0));
        // shader.setBool("use_sampler",true);
        // shader.setBool("is_waving",false);
        // shader.setInt("texture_sampler",boat_texture.GetSamplerId());

        // boat.draw(GL_TRIANGLES);

        // shader.setMat4("model",glm::value_ptr(tower_model_mat));
        // shader.setVec3("objectColor",glm::vec3(1.0,0,0));
        // shader.setBool("use_sampler",true);
        // shader.setBool("is_waving",false);
        // shader.setInt("texture_sampler",lighthouse_texture.GetSamplerId());

        // lighthouse.draw(GL_TRIANGLES);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}