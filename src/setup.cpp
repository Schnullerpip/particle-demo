#include"setup.h"

namespace {
    jule::setup setup;
}

namespace jule{
GLint window_x_coord = 0;
GLint window_y_coord = 0;
GLsizei window_width = 800;
GLsizei window_height = 600;
GLFWwindow *window = nullptr;


int init_all()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    jule::window = glfwCreateWindow(jule::window_width, jule::window_height, "particles-demo", NULL, NULL);
    if(jule::window == NULL)
    {
        printf("failed to create glfw window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(jule::window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("failed to initialize GLAD\n");
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(jule::window_x_coord, jule::window_y_coord, jule::window_width, jule::window_height);

    glfwSetFramebufferSizeCallback(jule::window, jule::framebuffer_size_callback);

    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    jule::window_width = width;
    jule::window_height = height;
    glViewport(jule::window_x_coord, jule::window_y_coord, width, height);
}
}//namespace jule