#version 450 core
layout (location = 0) in vec2 vertex; // <vec2 pos, vec2 tex>
layout (location = 1) in vec2 uvs;
out vec2 TexCoords;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * vec4(vertex,0.0, 1.0);
    TexCoords = uvs;
}