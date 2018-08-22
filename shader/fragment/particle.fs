#version 330 core

in vec2 pass_tex_coords;

out vec4 color;

uniform sampler2D u_texture_1;

void main()
{
    color = texture(u_texture_1, pass_tex_coords);

    if(color.x < 0.0008f) discard;

    color = vec4(1.0, 0.7, 0.4, color.x);
}