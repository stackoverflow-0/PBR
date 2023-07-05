#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTex;
uniform float t;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;
uniform bool is_waving;
uniform int wave_frame_id;
uniform sampler2D wave_height_sampler;
vec3 wave(vec3 pos) {

    if(is_waving) {
        float frame_num = 10.0f;
        // vec3 height = texture(wave_height_sampler, vec2(aTex.x, (aTex.y + wave_frame_id) / frame_num)).rgb;
        vec3 height = texture(wave_height_sampler, aTex).rgb;
        // float w = 0.08 * (1.5 * sin((2 * t + pos.x * 50 + pos.z * 35)) + 2* sin(2 * t +( pos.y + pos.x)* 50 ) + 2 * sin(2 * t + pos.z  * 35 + pos.y * 60));
        float w = 1;
        return pos + w * 0.08*  height.r * pos;
        
    }else {
        return pos;
    }
}

void main()
{
    gl_Position =  lightSpaceMatrix * model *vec4(wave(aPos),1.0);

}