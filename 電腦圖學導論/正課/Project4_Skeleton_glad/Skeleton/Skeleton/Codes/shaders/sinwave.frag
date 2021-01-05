#version 430 core
#extension GL_NV_shadow_samplers_cube : enable
out vec4 f_color;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

uniform SpotLight spotLight;
struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
 
uniform PointLight pointLights;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

in V_OUT
{
   vec3 position;
   vec3 normal;
   vec2 texture_coordinate;
} f_in;
 
uniform DirLight dirLight;

uniform vec3 viewPos;

uniform sampler2D u_texture;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);  
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

uniform bool dir_open;
uniform bool point_open;
uniform bool spot_open;

uniform bool reflect_open;
uniform bool refract_open;

uniform float Eta;
uniform float ratio_of_reflect_refract = 0.5f;

uniform samplerCube skybox;
uniform bool toon_open;

uniform sampler2D tiles;
const float poolHeight = 1;

const vec3 underwaterColor = vec3(0.4, 0.9, 1.0);
vec2 intersectCube(vec3 origin, vec3 ray, vec3 cubeMin, vec3 cubeMax) {
	vec3 tMin = (cubeMin - origin) / ray;
	vec3 tMax = (cubeMax - origin) / ray;
	vec3 t1 = min(tMin, tMax);
	vec3 t2 = max(tMin, tMax);
	float tNear = max(max(t1.x, t1.y), t1.z);
	float tFar = min(min(t2.x, t2.y), t2.z);
	return vec2(tNear, tFar);
}


vec3 getWallColor(vec3 point) {
	float scale = 0.5;

	vec3 wallColor;
	vec3 normal;
	if (abs(point.x) > 0.999) {
		wallColor = texture2D(tiles, point.yz * 0.5 + vec2(1.0, 0.5)).rgb;
		normal = vec3(-point.x, 0.0, 0.0);
	} else if (abs(point.z) > 0.999) {
		wallColor = texture2D(tiles, point.yx * 0.5 + vec2(1.0, 0.5)).rgb;
		normal = vec3(0.0, 0.0, -point.z);
	} else {
		wallColor = texture2D(tiles, point.xz * 0.5 + 0.5).rgb;
		normal = vec3(0.0, 1.0, 0.0);
	}

	scale /= length(point); /* pool ambient occlusion */
	

	return wallColor * scale;
}

  vec3 getSurfaceRayColor(vec3 origin, vec3 ray, vec3 waterColor) {
        vec3 color;
        if (ray.y < 0.0) {
		    vec2 t = intersectCube(origin, ray, vec3(-1, -poolHeight, -1), vec3(1, poolHeight, 1));
            color = getWallColor(origin + ray * t.y);
		}
        else{
            color = vec3(textureCube(skybox, ray)); 
        }

        if(ray.y<0.0f)color*=waterColor;
        return color;
  }


void main()
{   
    if(toon_open){
            float intensity;
            vec4 color;
 
            // normalizing the lights position to be on the safe side
            vec3 n = normalize(f_in.normal);
            vec3 lightDir = vec3(2,1,3);
            //vec3 lightDir = viewPos;
            intensity = dot(lightDir,n);
 
    if (intensity > 0.95)
        color = vec4(1.0,0.5,0.5,1.0);
    else if (intensity > 0.5)
        color = vec4(0.6,0.3,0.3,1.0);
    else if (intensity > 0.25)
        color = vec4(0.4,0.2,0.2,1.0);
    else
        color = vec4(0.2,0.1,0.1,1.0);
 
    f_color = color;

    }
    else{
    vec3 lighting ={0,0,0};
    vec3 viewDir = normalize(viewPos - f_in.position );

    if(dir_open) lighting += CalcDirLight(dirLight, f_in.normal, viewDir);
    if(point_open) lighting += CalcPointLight(pointLights, f_in.normal,f_in.position, viewDir);
    if(spot_open) lighting += CalcSpotLight(spotLight, f_in.normal, f_in.position, viewDir);


    vec3 texture_color = vec3(texture(texture_diffuse1, f_in.texture_coordinate));
    vec3 basecolor = texture_color + lighting;
    vec3 I = normalize(f_in.position - viewPos);
    vec3 ReflectVec = reflect(I, normalize(f_in.normal));
    vec3 RefractVec_up = refract(I, -normalize(f_in.normal),Eta);
    vec3 RefractVec_down = refract(I, normalize(f_in.normal),Eta);
    vec3 ReflectColor = vec3(textureCube(skybox, ReflectVec));
    vec3 RefractColor;
    if(viewPos.y > f_in.position.y){
        RefractColor = getSurfaceRayColor(vec3(f_in.texture_coordinate.x*2-1,0.0f,f_in.texture_coordinate.y*2-1), RefractVec_down, vec3(1.0)) * vec3(0.8, 1.0, 1.1);
    }
    else{
        RefractColor = getSurfaceRayColor(vec3(f_in.texture_coordinate.x*2-1,0.0f,f_in.texture_coordinate.y*2-1), RefractVec_up, vec3(1.0)) * vec3(0.8, 1.0, 1.1);
    }

    f_color = vec4(mix(ReflectColor,RefractColor ,ratio_of_reflect_refract),1.0f); 
    //else f_color = vec4(basecolor,1.0f);//這個會讓shader爆炸，原因不明，Reflect Refract似乎不能跟貼圖同時出現

    }
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
     //diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, f_in.texture_coordinate));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, f_in.texture_coordinate));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, f_in.texture_coordinate));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
     //diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
     //specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
     //attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
     //combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, f_in.texture_coordinate));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, f_in.texture_coordinate));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, f_in.texture_coordinate));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, f_in.texture_coordinate));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, f_in.texture_coordinate));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, f_in.texture_coordinate));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

//#version 330 core
//out vec4 FragColor;
//
//in vec2 TexCoords;
//
//uniform sampler2D texture_diffuse1;
//
//void main()
//{    
//    FragColor = texture(texture_diffuse1, TexCoords);
//}