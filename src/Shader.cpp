#include"../include/shader.h"

Shader *currently_used_shader = nullptr;

namespace jule
{
Shader * get_currently_bound_shader()
{
    return currently_used_shader;
}

void set_currently_bound_shader(Shader *s)
{

}
}//namespace jule
