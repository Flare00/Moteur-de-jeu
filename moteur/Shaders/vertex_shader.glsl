#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

//TODO create uniform transformations matrices Model View Projection
// Values that stay constant for the whole mesh.
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_camera_transformation;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;


void main(){
        FragPos = aPos;
        Normal = aNormal;
        TexCoord = aTexCoord;

        mat4 mvp = u_projection * u_view * u_model;
        gl_Position = u_camera_transformation * mvp * vec4(FragPos,1);
}

