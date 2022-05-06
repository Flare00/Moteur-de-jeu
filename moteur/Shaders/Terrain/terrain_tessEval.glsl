#version 450 core
layout(quads, fractional_odd_spacing, ccw) in;


uniform sampler2D u_heightmap;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_camera_transformation;

in vec2 TextureCoord[];
out vec2 TexCoord;

out vec3 FragPos;
out vec3 Normal;

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec2 t00 = TextureCoord[0];
    vec2 t01 = TextureCoord[1];
    vec2 t10 = TextureCoord[2];
    vec2 t11 = TextureCoord[3];

    vec2 t0 = (t01 - t00) * u + t00;
    vec2 t1 = (t11 - t10) * u + t10;
    TexCoord = (t1 - t0) * v + t0;

    float Height = texture(u_heightmap, vec2(-TexCoord.x, TexCoord.y)).y * 2.0f;

    vec4 p00 = gl_in[0].gl_Position;
    vec4 p01 = gl_in[1].gl_Position;
    vec4 p10 = gl_in[2].gl_Position;
    vec4 p11 = gl_in[3].gl_Position;

    vec4 uVec = p01 - p00;
    vec4 vVec = p10 - p00;
    vec4 normal = normalize( vec4(cross(vVec.xyz, uVec.xyz), 0) );

    vec4 p0 = (p01 - p00) * u + p00;
    vec4 p1 = (p11 - p10) * u + p10;
    vec4 p = (p1 - p0) * v + p0 + normal * Height;

    FragPos = vec3(u_model * p);
    Normal = normal.xyz;
    mat4 mvp = u_projection * u_view * u_model;
    gl_Position = u_camera_transformation * mvp * p;
}