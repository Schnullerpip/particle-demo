#ifndef jule_input_management_h
#define  jule_input_management_h

#include"main.h"
#include"observer.h"

class InputManager : public Observable<WASD_key_input>
{
    GLFWwindow *window;
    InputManager(GLFWwindow *window):window(window){}

    //different event data_assembly methods that need to be implemented
    WASD_key_input assemble_data() override
    {
        WASD_key_input wasd;
        wasd.w = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ? 1 : 0;
        wasd.a = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ? 1 : 0;
        wasd.s = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ? 1 : 0;
        wasd.d = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ? 1 : 0;
        return wasd;
    }

public:
    static InputManager * get_instance()
    {
        static InputManager *input_manager = new InputManager(jule::window);
        return input_manager;
    }
};


namespace jule{
void processInput(GLFWwindow *window);
}//namespace jule

#endif