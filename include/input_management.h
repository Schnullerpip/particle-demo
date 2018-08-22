#ifndef jule_input_management_h
#define  jule_input_management_h

#include"main.h"
#include"observer.h"

namespace jule{
class InputManager :
    public EventSource<WASD_key_input>,
    public EventSource<mouse_input>
{
    GLFWwindow *window;

    //EventSource<WASD_key_input>::
    WASD_key_input assemble_data(WASD_key_input *d) override;

    //EventSource<mouse_input>::
    mouse_input assemble_data(mouse_input *d)override;

public:
    mouse_input mi;

    static InputManager * get_instance()
    {
        static InputManager input_manager(jule::window);
        return &input_manager;
    }

    InputManager(GLFWwindow *window):window(window){}
    virtual ~InputManager(){}
};


void processInput(GLFWwindow *window);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
}//namespace jule

#endif