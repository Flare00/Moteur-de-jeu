#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 aPos;

//TODO create uniform transformations matrices Model View Projection
// Values that stay constant for the whole mesh.
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_camera_transformation;

void main(){
    mat4 mvp = u_projection * u_view * mat4(1);
    gl_Position = u_camera_transformation * mvp * vec4(aPos, 1.0f);
}

