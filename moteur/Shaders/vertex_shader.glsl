#version 450 core
#define MAX_LIGHT 28

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

//TODO create uniform transformations matrices Model View Projection
// Values that stay constant for the whole mesh.
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main(){
        vec4 p = (u_view * u_model) * vec4(aPos,1.0f);
        FragPos = p.xyz;

        FragPos = vec3(u_model * vec4(aPos, 1.0f));

        Normal = normalize(mat3(transpose(inverse(u_model))) * aNormal);
        TexCoord = aTexCoord;

        gl_Position = u_projection * p;
}

