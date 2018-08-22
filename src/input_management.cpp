#include"input_management.h"

namespace jule{

WASD_key_input InputManager::assemble_data(WASD_key_input *d)
{
    WASD_key_input wasd;
    wasd.w = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ? 1 : 0;
    wasd.a = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ? 1 : 0;
    wasd.s = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ? 1 : 0;
    wasd.d = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ? 1 : 0;
    wasd.shift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? 1 : 0;
    return wasd;
}

mouse_input InputManager::assemble_data(mouse_input *d)
{
    return mi;
}

pause InputManager::assemble_data(pause *p)
{
    return pause();
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }


    if(
        glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS )
    {
        InputManager::get_instance()->EventSource<WASD_key_input>::trigger();
    }


    static bool is_paused = false;
    int pause_button = glfwGetKey(window, GLFW_KEY_SPACE);
    if(pause_button == GLFW_PRESS && is_paused == false)
    {
        is_paused = true;
        InputManager::get_instance()->EventSource<pause>::trigger();
    }else if(pause_button == GLFW_RELEASE)
    {
        is_paused = false;
    }
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    static float lastX = jule::window_width/2, lastY = jule::window_height/2;
    InputManager *im = InputManager::get_instance();
    
    im->mi.xoffset = xpos - lastX;
    im->mi.yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    InputManager::get_instance()->EventSource<mouse_input>::trigger();
}
}//namespace jul