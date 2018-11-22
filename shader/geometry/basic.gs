#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in float pass_life[];

out vec2 pass_tex_coords;

uniform float max_life;
uniform float particle_radius;

const vec2 tex_coords[] = vec2 [] (
    vec2(0.0f, 0.0f),
    vec2(1.0f, 0.0f),
    vec2(0.0f, 1.0f),
    vec2(1.0f, 1.0f)
);

const vec2 coords[] = vec2 [] (
    vec2(-1.0f, -1.0f),
    vec2( 1.0f, -1.0f),
    vec2(-1.0f,  1.0f),
    vec2( 1.0f,  1.0f)
);

void main()
{
    vec4 pos = gl_in[0].gl_Position;

    for(int i = 0; i < 4; ++i)
    {
        gl_Position = pos + vec4(coords[i] * particle_radius, 0.0f, 0.0f);
        pass_tex_coords = tex_coords[i];
        EmitVertex();
    }
    EndPrimitive();
}