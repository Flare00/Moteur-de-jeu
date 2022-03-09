#version 330 core
//vertex_shader.glsl part
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertices_position_modelspace;
layout(location = 1) in vec2 aTexCoord;

//TODO create uniform transformations matrices Model View Projection
// Values that stay constant for the whole mesh.
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_camera_transformation;

uniform mat4 u_model_transformation;

out vec3 localColor;
out vec2 TexCoord;

//Terrain specification
uniform sampler2D u_heightmap;
uniform int u_has_heightmap;
out float height;


void main(){
        bool heightSet = false;
        if(u_has_heightmap > 0){
                height = 2.0 - texture2D(u_heightmap, aTexCoord).r * 2.0;
                heightSet = true;
        }

        mat4 mvp = u_projection * u_view * u_model;

        if(heightSet){
            gl_Position = u_camera_transformation * mvp * vec4(vertices_position_modelspace,1) + vec4(0,height,0,0);
            localColor =  vec3(height,0,0);
        } else {
            gl_Position = u_camera_transformation * mvp * vec4(vertices_position_modelspace,1);
            height = vertices_position_modelspace.y;
            localColor =  vec3(0,0,1);
        }

        TexCoord = aTexCoord;
}

