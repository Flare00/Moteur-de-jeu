#version 450 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D u_text;
uniform vec3 u_textColor;

void main()
{    
    //color = (vec4(1) * texture2D(u_text, vec2(TexCoords.x, TexCoords.y)));

    color = vec4(u_textColor, texture2D(u_text, vec2(TexCoords.x, TexCoords.y)).r);
}