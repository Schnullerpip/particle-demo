#version 330 core
in vec2 pass_tex_coords;

uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2;

out vec4 out_color;

void main()
{
	//out_color = vec4(pass_color, 1.0f);
	out_color = mix(texture(u_texture_1, pass_tex_coords), texture(u_texture_2, pass_tex_coords), 0.2f);
}