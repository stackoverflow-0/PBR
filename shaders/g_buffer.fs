#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
// layout (location = 2) out vec3 gFragPosLightSpace;

in vec2 uvCoord;
in vec3 FragPos;
in vec3 normal;
in vec4 FragPosLightSpace;

uniform sampler2D shadowMap;
// uniform sampler2D texture_diffuse1;
// uniform sampler2D texture_specular1;
uniform bool use_sampler;
uniform sampler2D texture_sampler;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth + 0.001  ? 1.0 : 0.0;
    if(projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
} 

void main()
{    
    // Store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // Also store the per-fragment normals into the gbuffer
    gNormal = normalize(normal);

    float shadow = ShadowCalculation(FragPosLightSpace);
    
    if(use_sampler) {
        // texture_sampler = use_sampler_id;
        gAlbedoSpec = vec4(texture(texture_sampler,vec2(uvCoord.x,1-uvCoord.y)).rgb * (1 - 0.5 * shadow),shadow);
        // FragColor = vec4(texture(texture_sampler,vec2(uvCoord.x,1-uvCoord.y)).rgb * (1 - 0.5 * shadow),shadow);
    }
    else {
        gAlbedoSpec = vec4(-1,-1,-1,shadow);
        // FragColor = vec4(0,0,0,1);
    }
    // And the diffuse per-fragment color
    // gAlbedoSpec.rgb = texture(texture_diffuse1, uvCoord).rgb;
    // Store specular intensity in gAlbedoSpec's alpha component
    // gAlbedoSpec.a = texture(texture_specular1, uvCoord).r;
    
    // gFragPosLightSpace = FragPosLightSpace;
}