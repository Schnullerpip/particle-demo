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

        collision_data cd;
        cd.colliding = true;
        cd.collision_point = collision_point;
        cd.dir_vec_from_p1_to_collision_point = dir_to_collision_point_from_p2 * -1.f;
        cd.dir_vec_from_p2_to_collision_point = dir_to_collision_point_from_p2;
        cd.distance_from_p1_to_collision_point = distance_to_collision_point_from_pos1;
        cd.distance_from_p2_to_collision_point = distance_to_collision_point_from_pos2;
        cd.merge_distance = diff;

        return cd;
    }

    return {};
}
}//namespace jule