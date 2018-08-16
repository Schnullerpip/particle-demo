#ifndef jule_main_h
#define jule_main_h

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<vector>
#include<map>
namespace jule{
extern GLint window_x_coord;
extern GLint window_y_coord;
extern GLsizei window_width;
extern GLsizei window_height;
extern GLFWwindow *window;

extern float delta_time;
}
#endif

