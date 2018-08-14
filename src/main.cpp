#include"setup.h"

int main(){

    if(jule::init_all() < 0)
    {
        return -1;
    }

    while(!glfwWindowShouldClose(jule::window))
    {
        glfwSwapBuffers(jule::window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}