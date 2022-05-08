#version 450 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aPos;

//TODO create uniform transformations matrices Model View Projection
// Values that stay constant for the whole mesh.
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main(){
    mat4 mvp = u_projection * u_view * u_model;
    gl_Position = mvp * vec4(aPos, 1.0f);
}

