#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTex;

out vec3 FragPos;
out vec2 uvCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    uvCoord = aTex;
    FragPos = vec3(model * vec4(aPos, 1));
    gl_Position = projection * view * model * vec4(aPos, 1);

}