#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 t_normal;

in vec4 FragPosLightSpace;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos; 
uniform vec3 viewPos;

uniform sampler2D shadowMap;

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
    // 漫反射照射光
    // 确定来光方向-->在法向量上投影
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 norm = normalize(t_normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    // 环境光
    float ambientStrength = 0.6;
    vec3 ambient = ambientStrength * lightColor;
    // 镜面高光
    // 确定观察方向-->根据来光方向和面法向确定反射方向-->根据方向计算光强
    float specularStrength = 0.1;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(t_normal, halfwayDir), 0.0), 256);

    vec3 specular = specularStrength * spec * lightColor;  

    float shadow = ShadowCalculation(FragPosLightSpace);
    vec3 result = (ambient + (1.0 - shadow) *(diffuse + specular)) * objectColor;

    FragColor = vec4(result, 1.0);
    // FragColor = vec4(1,0,0,1);
}