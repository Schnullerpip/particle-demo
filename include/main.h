#ifndef jule_main_h
#define jule_main_h

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>


#include<vector>
#include<map>
#include<random>

namespace jule{
extern GLint window_x_coord;
extern GLint window_y_coord;
extern GLsizei window_width;
extern GLsizei window_height;
extern GLFWwindow *window;

extern float delta_time;

extern std::random_device rd;
extern std::mt19937 gen;

float get_rnd_float(float min, float max);
}//namespace jule


//event data used to decouple camera and input management
struct WASD_key_input {
    int w;
    int a;
    int s;
    int d;
    int shift = 0;
};

struct mouse_input
{
    float xoffset;
    float yoffset;
};


#endif

