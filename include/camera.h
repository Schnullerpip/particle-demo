#ifndef jule_camera_h
#define jule_camera_h

#include"observer.h"

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

class Camera : public Observer<WASD_key_input>
{
    glm::vec3 camera_pos     = glm::vec3(0.f, 0.f, 3.f);
    glm::vec3 camera_front   = glm::vec3(0.f, 0.f, -1.f);
    glm::vec3 camera_up      = glm::vec3(0.f, 1.f, 0.f);

    float camera_speed = 1.5f;

    void callback(WASD_key_input wasd) override 
    {
        glm::vec3 camera_direction = glm::vec3(0.f, 0.f, 0.f);
        if(wasd.w)
            camera_direction +=  camera_front;
        if(wasd.s)
            camera_direction -= camera_front;
        if(wasd.a)
            camera_direction -= glm::normalize(glm::cross(camera_front, camera_up));
        if(wasd.d)
            camera_direction += glm::normalize(glm::cross(camera_front, camera_up));
        
        camera_pos += camera_direction * camera_speed * jule::delta_time;

    }


public:
    glm::mat4 update()
    {
        glm::mat4 view;
        view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
        return view;
    }
};

#endif