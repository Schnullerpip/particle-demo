#ifndef jule_setup_h
#define jule_setup_h

#include<stdio.h>
#include<stdlib.h>
#include"main.h"


namespace jule {
/*
will initialize all the gl stuff i dont want to see in the relevant code
*/
int init_all();

//callbacks
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

struct setup{
    setup(){
        init_all();
    }
};

}//namespace jule


#endif