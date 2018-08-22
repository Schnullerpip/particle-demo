#version 330 core
#define baby_age 0.03f
#define old_age 0.09f
#define PI 3.41

in vec2 pass_tex_coords;
in float fs_life;

out vec4 color;

uniform sampler2D u_texture_1;

void main()
{
    color = texture(u_texture_1, pass_tex_coords);
    if(color.x < 0.0005f) discard;

    float life = 1-fs_life;
    if(life < baby_age)
    {
        color = vec4(1.f, 1.f, 1.f, color.x);
    }
    else
    {
        color = vec4(cos(life*PI)+1.f, cos(life*PI)-0.2f, 0.f, color.x);
    }
}