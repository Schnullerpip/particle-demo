#version 330 core
layout(location = 0) in vec3 in_pos;
layout(location = 1) in float in_life;
layout(location = 2) in vec3 in_color;

out float pass_life;
out vec3 vs_color;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
	pass_life = in_life;
	vs_color = in_color;
	gl_Position = u_projection * u_view * vec4(in_pos, 1.0f);
}