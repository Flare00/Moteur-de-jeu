#version 330 core
//fragment_shader.glsl part
// Ouput data
out vec4 color;
in vec3 localColor;
in vec2 TexCoord;


uniform sampler2D u_texture;
uniform int u_has_texture = 0;

//terrain specification
in float height;

uniform sampler2D u_texture_top;
uniform sampler2D u_texture_bottom;

uniform float u_limit_top;
uniform float u_limit_bottom;
uniform float u_limit_blur;

void main(){

	if(u_has_texture == 0){
		color = vec4(localColor ,1 );
	} else {
		if(height < u_limit_bottom - u_limit_blur){
			color = texture2D(u_texture_bottom, TexCoord);
		} else if (height < u_limit_bottom + u_limit_blur){
		//(height - u_min_height) /(u_middle_low_height - u_min_height)
			float minimal = u_limit_bottom - u_limit_blur;
			float maximal = u_limit_bottom + u_limit_blur;

			color = mix(texture2D(u_texture_bottom, TexCoord), texture2D(u_texture, TexCoord), (height - minimal) / (maximal - minimal) );
		} else if (height < u_limit_top - u_limit_blur){
			color = texture2D(u_texture, TexCoord);
		} else if (height < u_limit_top + u_limit_blur){
			float minimal = u_limit_top - u_limit_blur;
			float maximal = u_limit_top + u_limit_blur;
			color = mix(texture2D(u_texture, TexCoord), texture2D(u_texture_top, TexCoord), (height - minimal) / (maximal - minimal));
		} else {
			color = texture2D(u_texture_top, TexCoord);
		}


	}

}
