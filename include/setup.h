#ifndef jule_setup_h
#define jule_setup_h

#include<stdio.h>
#include<stdlib.h>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

namespace jule {
extern GLint window_x_coord;
extern GLint window_y_coord;
extern GLsizei window_width;
extern GLsizei window_height;
extern GLFWwindow *window;


/*
will initialize all the gl stuff i dont want to see in the relevant code
*/
int init_all();

//callbacks
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

}//namespace jule


#endif