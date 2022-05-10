#version 330 core
#define MAX_LIGHT 28

// Ouput data
out vec4 color;

struct Material {
    vec3 ambiant;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
    vec3 light_dir;

    int generate_shadows;
	sampler2D shadow_map;
    mat4 light_matrix;
};

// Input data
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

//uniform data
uniform Material u_material;
uniform Light u_lights[MAX_LIGHT];
uniform vec3 u_camera_pos;


uniform float u_gamma_correction;

uniform sampler2D u_textures[4];
uniform int u_nb_texture = 0;
uniform int u_light_number = 0;

uniform float u_mult_shader;

float computeShadow(Light light, vec3 normal, vec3 lightdir){
    vec4 fragPosLightSpace = light.light_matrix * vec4(FragPos,1.0f);
    float shadow = 0.0f;
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = (projCoords * 0.5) + 0.5;

    float closestDepth = texture(light.shadow_map, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    float bias = max(0.01 * (1.0 - dot(normal, lightdir)), 0.001);
    //shadow = currentDepth -bias > closestDepth ? 1.0 : 0.0;
    vec2 texelSize = 1.0 / textureSize(light.shadow_map, 0);
    int flou_res = 8;

    for(int x = -flou_res; x <= flou_res; ++x)
    {
        for(int y = -flou_res; y <= flou_res; ++y)
        {
            float pcfDepth = texture(light.shadow_map, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= (flou_res * flou_res) + 1.0f;

    if(projCoords.z > 1.0)
        shadow = 0.0;


    return shadow;
}

vec3 calculateLight(Light light, vec3 normal, vec3 cameraDir){
    vec3 lightDir =  normalize(light.position - light.light_dir);
    vec3 lightToFragment = normalize(light.position - FragPos);
    if(lightDir.x == 0 && lightDir.y == 0 && lightDir.z == 0){
        lightDir = lightToFragment;
    } 

    //Attenuation
    float angleAttenuation = dot(lightDir, lightToFragment);

    float attenuation = min(1.0 / distance(light.position, FragPos), 1.0);

    //Ambient
    vec3 ambient = light.color * light.intensity * u_material.ambiant;

    if(angleAttenuation <=0 || attenuation <= 0.001f){
        return ambient;
    }

    float shadow = 0.0f;
    //Shadow
    if(light.generate_shadows == 1){
       shadow = computeShadow(light, normal, lightDir);
    }

    vec3 lCI = light.color * light.intensity * attenuation * angleAttenuation;

    //diffuse
    float diff = max(dot(normal, lightToFragment), 0.0);
    vec3 diffuse = diff * lCI * u_material.diffuse;
    //speculaire
    vec3 reflectDir = reflect(-lightToFragment, normal);
    float spec = pow(max(dot(cameraDir, reflectDir),0.0), 32);

    vec3 specular = spec * lCI * u_material.specular;
    return (ambient + ((1.0f-shadow) * (diffuse + specular)));
}

void main(){
    //color = vec4(u_lights[0].intensity,0,0, 1);
    vec3 norm = normalize(Normal);
    vec3 cameraDir = normalize(u_camera_pos - FragPos);

    vec3 lightVec = vec3(0.0f);
    vec4 FragPosLight;
    for(int i = 0; i < u_light_number; i++){
        lightVec += calculateLight(u_lights[i], norm, cameraDir);
    }
    lightVec = pow(lightVec, vec3(1.0/u_gamma_correction)); // GAMMA Correction


	if(u_nb_texture > 0){
		color = vec4(lightVec,1.0f) * (texture2D(u_textures[0], vec2(-TexCoord.x, TexCoord.y)));

	} else {
        color = vec4(lightVec,1.0f) * vec4(TexCoord.x, TexCoord.y, 0,1);
	}
}

