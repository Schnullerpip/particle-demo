#version 330 core
#define baby_age 0.03f
#define old_age 0.09f
#define PI 3.41

in vec2 pass_tex_coords;
in float fs_life;
in vec3 gs_color;

out vec4 color;

uniform sampler2D u_texture_1;

void main()
{
    color = texture(u_texture_1, pass_tex_coords);
    if(color.x < 0.0005f) discard;

    color = vec4(gs_color, color.x);
}