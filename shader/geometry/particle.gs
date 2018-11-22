#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in float pass_life[];

out vec2 pass_tex_coords;
out float fs_life;

uniform float max_life;
uniform float particle_radius;

const vec2 tex_coords[] = vec2 [] (
    vec2(0.0f, 0.0f),
    vec2(1.0f, 0.0f),
    vec2(0.0f, 1.0f),
    vec2(1.0f, 1.0f)
);

//const vec2 coords[] = vec2 [] (
//    vec2(-0.1f, -0.1f),
//    vec2( 0.1f, -0.1f),
//    vec2(-0.1f,  0.1f),
//    vec2( 0.1f,  0.1f)
//);
const vec2 coords[] = vec2 [] (
    vec2(-1.0f, -1.0f),
    vec2( 1.0f, -1.0f),
    vec2(-1.0f,  1.0f),
    vec2( 1.0f,  1.0f)
);

void main()
{
    vec4 pos = gl_in[0].gl_Position;

    //grows bigger towards 1
    float life_factor =  pass_life[0]/max_life;
    float scale_factor = cos((life_factor)*3.41f);

    for(int i = 0; i < 2; ++i)
    {
        gl_Position = pos + vec4(coords[i] * particle_radius * scale_factor, 0.0f, 0.0f);
        pass_tex_coords = tex_coords[i];
        fs_life = life_factor;
        EmitVertex();
    }
    for(int i = 2; i < 4; ++i)
    {
        gl_Position = pos + vec4(coords[i] * particle_radius * (scale_factor), 0.0f, 0.0f);
        pass_tex_coords = tex_coords[i];
        fs_life = life_factor;
        EmitVertex();
    }
    EndPrimitive();
}