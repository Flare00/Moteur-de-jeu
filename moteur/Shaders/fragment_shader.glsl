#version 330 core

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
};

// Input data
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

#define NB_LIGHTS 1
//uniform data
uniform Material u_material;
uniform Light u_lights[NB_LIGHTS];
uniform vec3 u_camera_pos;

uniform sampler2D u_texture;
uniform int u_has_texture = 0;

vec3 calculateLight(Light light, vec3 normal, vec3 cameraDir){
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(cameraDir, reflectDir),0.0), u_material.shininess);
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / distance;

    vec3 lCI = light.color * light.intensity;
    vec3 ambient = lCI * u_material.ambiant;
    vec3 diffuse = diff * lCI * u_material.diffuse;
    vec3 specular = spec * lCI * u_material.specular;
    return (ambient + diffuse + specular);
}

void main(){

    vec3 norm = normalize(Normal);
    vec3 cameraDir = normalize(u_camera_pos - FragPos);

    vec3 lightVec = vec3(1.0f);
    for(int i = 0; i < NB_LIGHTS; i++){
            lightVec += calculateLight(u_lights[i], norm, cameraDir);
    }
	if(u_has_texture == 0){
		color = (vec4(lightVec,1) * vec4(TexCoord , 0, 1 ));
	} else {
		color = (vec4(lightVec,1) * texture2D(u_texture, vec2(-TexCoord.x, TexCoord.y)));
	}
}

