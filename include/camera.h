#ifndef jule_h
#define jule_h

#include"observer.h"

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

class Camera :
    public Observer<WASD_key_input>,
    public Observer<mouse_input>
{
    glm::vec3 pos     = glm::vec3(0.f, 0.f, 3.f);
    glm::vec3 front   = glm::vec3(0.f, 0.f, -1.f);
    glm::vec3 up      = glm::vec3(0.f, 1.f, 0.f);
    glm::vec3 right   = glm::vec3(1.f, 0.f, 0.f);

    float yaw = -90.f;
    float pitch = 0.0f;

    float speed = 1.5f;
    float sensitivity = 1.5f;

    event_callback(WASD_key_input, handle)
    void handle(WASD_key_input wasd)
    {
        glm::vec3 direction = glm::vec3(0.f, 0.f, 0.f);
        if(wasd.w) direction +=  front;
        if(wasd.s) direction -= front;
        if(wasd.a) direction -= glm::normalize(glm::cross(front, up));
        if(wasd.d) direction += glm::normalize(glm::cross(front, up));
        
        pos += direction * speed * jule::delta_time;
    }

    event_callback(mouse_input, handle)
    void handle(mouse_input mi)
    {
        float factor = sensitivity * jule::delta_time;
        mi.xoffset *= factor;
        mi.yoffset *= factor;

        yaw += mi.xoffset;
        pitch += mi.yoffset;

        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;
        
        updateCameraVectors();
    }

    void updateCameraVectors()
    {
        glm::vec3 new_front;
        new_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        new_front.y = sin(glm::radians(pitch));
        new_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        front = glm::normalize(new_front);
        right = glm::normalize(glm::cross(front, glm::vec3(0.f, 1.f, 0.f)));
        up    = glm::normalize(glm::cross(right, front));
    }


public:
    glm::mat4 update()
    {
        return glm::lookAt(pos, pos + front, up);
    }
};

#endif