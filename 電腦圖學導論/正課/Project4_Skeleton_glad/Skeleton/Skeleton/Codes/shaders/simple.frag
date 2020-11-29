//////#version 430 core
//////out vec4 f_color;
//////
//////in V_OUT
//////{
//////   vec3 position;
//////   vec3 normal;
//////   vec2 texture_coordinate;
//////} f_in;
//////
//////uniform vec3 u_color;
//////
//////uniform sampler2D u_texture;
//////
//////void main()
//////{   
//////    vec3 color = vec3(texture(u_texture, f_in.texture_coordinate));
//////    f_color = vec4(color, 1.0f);
//////}
////
////
////
////
//#version 430 core
//out vec4 f_color;
//
//struct DirLight {
//    vec3 direction;
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//};  
//
//struct SpotLight {
//    vec3 position;
//    vec3 direction;
//    float cutOff;
//    float outerCutOff;
//  
//    float constant;
//    float linear;
//    float quadratic;
//  
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;       
//};
//uniform SpotLight spotLight;
//struct PointLight {    
//    vec3 position;
//    
//    float constant;
//    float linear;
//    float quadratic;  
//
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//};  
// 
//uniform PointLight pointLights;
//
//struct Material
//{
//    sampler2D diffuse;
//    sampler2D specular;
//    float shininess;
//};
//uniform Material material;
//
//in V_OUT
//{
//   vec3 position;
//   vec3 normal;
//   vec2 texture_coordinate;
//} f_in;
// 
//uniform DirLight dirLight;
//
//uniform vec3 u_color;
//
//uniform vec3 viewPos;
//
//uniform sampler2D u_texture;
//
//uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_specular1;
//uniform sampler2D texture_normal1;
//uniform sampler2D texture_height1;
//
//vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);  
//vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
//vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
//
//void main()
//{   
//    vec3 result={0,0,0};
//    vec3 viewDir = normalize(viewPos - f_in.position);
//
//    //result += CalcDirLight(dirLight, f_in.normal, viewDir);
//    //result += CalcPointLight(pointLights, f_in.normal,f_in.position, viewDir);
//    //result += CalcSpotLight(spotLight, f_in.normal, f_in.position, viewDir);
//    vec3 color = vec3(texture(u_texture, f_in.texture_coordinate));
//    f_color = vec4(color+result, 1.0f);
//
//    f_color  = vec4(texture(texture_diffuse1,f_in.texture_coordinate));
//}
//
//vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
//{
//    vec3 lightDir = normalize(-light.direction);
//    // diffuse shading
//    float diff = max(dot(normal, lightDir), 0.0);
//    // specular shading
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    // combine results
//    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, f_in.texture_coordinate));
//    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, f_in.texture_coordinate));
//    vec3 specular = light.specular * spec * vec3(texture(material.specular, f_in.texture_coordinate));
//    return (ambient + diffuse + specular);
//}
//
//vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
//{
//    vec3 lightDir = normalize(light.position - fragPos);
//    // diffuse shading
//    float diff = max(dot(normal, lightDir), 0.0);
//    // specular shading
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    // attenuation
//    float distance    = length(light.position - fragPos);
//    float attenuation = 1.0 / (light.constant + light.linear * distance + 
//  			     light.quadratic * (distance * distance));    
//    // combine results
//    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, f_in.texture_coordinate));
//    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, f_in.texture_coordinate));
//    vec3 specular = light.specular * spec * vec3(texture(material.specular, f_in.texture_coordinate));
//    ambient  *= attenuation;
//    diffuse  *= attenuation;
//    specular *= attenuation;
//    return (ambient + diffuse + specular);
//}
//
//vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
//{
//    vec3 lightDir = normalize(light.position - fragPos);
//    // diffuse shading
//    float diff = max(dot(normal, lightDir), 0.0);
//    // specular shading
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    // attenuation
//    float distance = length(light.position - fragPos);
//    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
//    // spotlight intensity
//    float theta = dot(lightDir, normalize(-light.direction)); 
//    float epsilon = light.cutOff - light.outerCutOff;
//    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
//    // combine results
//    vec3 ambient = light.ambient * vec3(texture(material.diffuse, f_in.texture_coordinate));
//    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, f_in.texture_coordinate));
//    vec3 specular = light.specular * spec * vec3(texture(material.specular, f_in.texture_coordinate));
//    ambient *= attenuation * intensity;
//    diffuse *= attenuation * intensity;
//    specular *= attenuation * intensity;
//    return (ambient + diffuse + specular);
//}
//

#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{    
    FragColor = texture(texture_diffuse1, TexCoords);
}