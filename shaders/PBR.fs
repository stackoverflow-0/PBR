#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec2 uvCoord;
// in vec4 FragPosLightSpace;


const float PI = 3.14159265359;


// uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos; 
uniform vec3 viewPos;

uniform sampler2D shadowMap;
// uniform samplerCube skybox;

uniform  mat4 model_inv_T;
// material parameters
uniform sampler2D albedoMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;
uniform sampler2D normalMap;

uniform mat4 lightSpaceMatrix;

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

  
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}


void main()
{		
    vec2 muvCoord = vec2(uvCoord.x, 1 - uvCoord.y);
    vec3  albedo = texture(albedoMap, muvCoord).rgb;
    float metallic = texture(metallicMap, muvCoord).r;
    float roughness = texture(roughnessMap, muvCoord).r;
    // Note : we use glossiness , roughness = 1 - glossiness
    // roughness = 1.0 - roughness;
    float ao = texture(aoMap, muvCoord).r;
    // float ao = 0.1;

    vec3 normal = texture(normalMap, muvCoord).xyz;

    normal = mat3(model_inv_T) * normal;
    vec3 N = normalize(normal);
    vec3 V = normalize(viewPos - FragPos);

    vec3 F0 = vec3(0.04, 0.04, 0.04) * (1 - metallic) + albedo * metallic; 
    F0 = mix(F0, albedo, metallic);
	           
    // reflectance equation
    vec3 Lo = vec3(0.0);
    // for(int i = 0; i < 4; ++i) 
    // {
        // calculate per-light radiance
    vec3 L = normalize(lightPos - FragPos);
    vec3 H = normalize(V + L);
    float dis    = length(lightPos - FragPos);
    float attenuation = 1.0 / (dis * dis);
    vec3 radiance     = lightColor * attenuation;        
    
    // cook-torrance brdf
    float NDF = DistributionGGX(N, H, roughness);        
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
    
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;	  
    
    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular     = numerator / denominator;  
        
    // add to outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);                
    Lo += (kD * albedo / PI + specular) * radiance * NdotL; 
    // }   
  
    vec3 ambient = vec3(0.3) * albedo * ao ;

    vec4 FragPosLightSpace = lightSpaceMatrix * vec4(FragPos,1.0);

    float shadow = ShadowCalculation(FragPosLightSpace);

    vec3 R = reflect(-V, N);

    // Lo += texture(skybox, R).rgb;
    // Lo /= 2;

    vec3 color = ambient + Lo;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  

    FragColor = vec4(color ,1);
    // FragColor = vec4(0,0,0,1);
}  






// void main()
// {
//     // 漫反射照射光
//     // 确定来光方向-->在法向量上投影
//     vec3 lightDir = normalize(lightPos - FragPos);
//     vec3 norm = normalize(normal);
//     float diff = max(dot(norm, lightDir), 0.0);
//     vec3 diffuse = diff * lightColor;
//     // 环境光
//     float ambientStrength = 0.6;
//     vec3 ambient = ambientStrength * lightColor;
//     // 镜面高光
//     // 确定观察方向-->根据来光方向和面法向确定反射方向-->根据方向计算光强
//     float specularStrength = 0.1;
//     vec3 viewDir = normalize(viewPos - FragPos);
//     vec3 reflectDir = reflect(-lightDir, norm);
    

//     vec3 halfwayDir = normalize(lightDir + viewDir);
//     float spec = pow(max(dot(normal, halfwayDir), 0.0), 256);

//     vec3 specular = specularStrength * spec * lightColor;  

//     float shadow = ShadowCalculation(FragPosLightSpace);
//     vec3 result = (ambient + (1.0 - shadow) *(diffuse + specular)) * objectColor;
//     if(is_sun) {
//         FragColor = vec4(1.0,1.0,1.0,1.0);
//     }else {
//         FragColor = vec4(result, 1.0);
//     }
    

// }