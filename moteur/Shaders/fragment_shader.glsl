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

#define MAX_LIGHT 128
//uniform data
uniform Material u_material;
uniform Light u_lights[MAX_LIGHT];
uniform vec3 u_camera_pos;

uniform int u_light_number;
uniform float u_gamma_correction;

uniform sampler2D u_textures[32];
uniform int u_nb_texture = 0;

vec3 calculateLight(Light light, vec3 normal, vec3 cameraDir){

    float attenuation = 1.0 / distance(light.position, FragPos);
    vec3 lCI = light.color * light.intensity * attenuation;
    //ambiant
    vec3 ambient = lCI * u_material.ambiant;
    //diffuse
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lCI * u_material.diffuse;
    //speculaire
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(cameraDir, reflectDir),0.0), 32);

    vec3 specular = spec * lCI * u_material.specular;
    return (ambient + diffuse + specular);
}

void main(){
    //color = vec4(u_lights[0].intensity,0,0, 1);
    vec3 norm = normalize(Normal);
    vec3 cameraDir = normalize(u_camera_pos - FragPos);

    vec3 lightVec = vec3(0.0f);

    for(int i = 0; i < u_light_number; i++){
        lightVec += calculateLight(u_lights[i], norm, cameraDir);
    }
    lightVec = pow(lightVec, vec3(1.0/u_gamma_correction));

    lightVec = vec3(1.0f); //Temporary

	if(u_nb_texture == 0){
		color = (vec4(lightVec,1) * vec4(TexCoord , 0, 1 ));
	} else {
		color = (vec4(lightVec,1) * texture2D(u_textures[0], vec2(-TexCoord.x, TexCoord.y)));
	}

    /*if(gl_FrontFacing){
        color = vec4(1,0,0,1);
    } else {
        color = vec4(0,1,0,1);
    
    }*/

}

