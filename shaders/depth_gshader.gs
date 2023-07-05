#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;


in vec4 Pos0[];
in vec4 Pos1[];
in vec4 Pos2[];

void main() {
    gl_Position = Pos0[0];
    EmitVertex();
    gl_Position = Pos1[0];
    EmitVertex();
    gl_Position = Pos2[0];
    EmitVertex();
    EndPrimitive();
}