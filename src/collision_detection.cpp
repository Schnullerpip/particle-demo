#include "../include/collision_detection.h"

namespace jule{


const collision_data sphere_collision(glm::vec3 pos1, float rad1, glm::vec3 pos2, float rad2)
{
    float collision_distance = rad1+rad2;

    glm::vec3 vec_distance = pos1 - pos2;
    float point_distance = glm::length(vec_distance);

    float diff = collision_distance - point_distance;

    if(diff > (0.f - EPSILON))
    {
        //those two spheres collide!

        //the spheres overlap diff units
        //calculate the point of collision - from pos2 to pos1
        float distance_to_collision_point_from_pos2 = rad2 - diff/2;
        float distance_to_collision_point_from_pos1 = point_distance - distance_to_collision_point_from_pos2;

        glm::vec3 dir_to_collision_point_from_p2 = glm::normalize(vec_distance);

        glm::vec3 collision_point = dir_to_collision_point_from_p2 * distance_to_collision_point_from_pos2;

        return {
        true,
        collision_point,
        dir_to_collision_point_from_p2 * -1.f,
        dir_to_collision_point_from_p2,
        distance_to_collision_point_from_pos1,
        distance_to_collision_point_from_pos2,
        diff
        };
    }

    return {};
}
}//namespace jule