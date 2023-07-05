#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
#define G 1.2
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

in vec2 uvCoords[];
out vec2 uvCoord;
out vec3 normal;

out vec3 FragPos;

out vec4 FragPosLightSpace;

void main(){
    
    vec4 p0,p1,p2;
    p0 = gl_in[0].gl_Position;
    p1 = gl_in[1].gl_Position;
    p2 = gl_in[2].gl_Position;
    vec3 norm = cross(vec3(p0-p1),vec3(p0-p2));
    normal = mat3(transpose(inverse(model))) * norm;

    FragPos = vec3(model * p0);
    gl_Position = projection * view * model * p0;
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos,1.0);
    uvCoord = uvCoords[0];
    EmitVertex();

    FragPos = vec3(model * p1);
    gl_Position = projection * view * model * p1;
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos,1.0);
    uvCoord = uvCoords[1];
    EmitVertex();

    FragPos = vec3(model * p2);
    gl_Position = projection * view * model * p2;
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos,1.0);
    uvCoord = uvCoords[2];
    EmitVertex();

    EndPrimitive();
}