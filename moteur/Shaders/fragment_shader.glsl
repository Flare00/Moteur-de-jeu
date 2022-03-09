#version 330 core

// Ouput data
out vec4 color;
in vec3 localColor;
in vec2 TexCoord;
in float height;

uniform sampler2D u_texture;
uniform int u_has_texture = 0;


void main(){
	if(u_has_texture == 0){
		color = vec4(TexCoord , 0, 1 );
	} else {
		color = texture2D(u_texture, vec2(-TexCoord.x, TexCoord.y));
	}
}
