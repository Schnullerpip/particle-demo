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
bool get_rnd_bool();
int get_rnd_int(int min, int max);

extern bool running;

//a tolerance for float calculations
extern float EPSILON;

//predefined colors
constexpr float RED[3] =   {1.f, 0.f, 0.f};
constexpr float GREEN[3] = {0.f, 1.f, 0.f};
constexpr float BLUE[3] =  {0.f, 0.f, 1.f};

constexpr float BLACK[3] =  {0.f, 0.f, 0.f};
constexpr float WHITE[3] =  {1.f, 1.f, 1.f};

constexpr float ORANGE[3] =  {1.f, .6f, .3f};

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

struct pause {};

//utility functions
namespace jule{
    glm::vec3 rotateAround(glm::vec3 &pointToRotate, glm::vec3 &rotationCenter, glm::mat4 &rotation);

    glm::vec3 project_on(glm::vec3 v1, glm::vec3 v2);
}


#endif

