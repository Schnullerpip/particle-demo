#ifndef JULE_COLLISION_DETECTION
#define JULE_COLLISION_DETECTION

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<tuple>

#include"main.h"

struct collision_data
{
    bool colliding = false;
    glm::vec3 collision_point;
    glm::vec3 dir_vec_from_p1_to_collision_point;
    glm::vec3 dir_vec_from_p2_to_collision_point;
    float distance_from_p1_to_collision_point = 0.f;
    float distance_from_p2_to_collision_point = 0.f;
    float merge_distance = 0.f;
};

namespace jule {

//calculates the collision point of two points pos1 & pos2 and returns a tuple. if the first member of the tuple is true than the second is their point of collision, third and fourth are distances from pos1 and pos2 to collision point
const collision_data sphere_collision(glm::vec3 pos1, float rad1, glm::vec3 pos2, float rad2);

}//namespace jule

#endif