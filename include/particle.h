#ifndef jule_particle_h
#define  jule_particle_h

#include<algorithm>
#include<glm/gtx/norm.hpp>

#include"main.h"

enum yes_no {YES, NO};
enum ALIVE {alive, dead};
enum PARTICLE_ATTRIB {x, y, z, life};

#define THRESHOLD 0.0005f

struct particle
{
    //position
    glm::vec3 m_xyz = glm::vec3(0.f, 0.f, 0.f);

    //acceleration
    glm::vec3 m_acc = glm::vec3(0.f, 0.f, 0.f);

    //velocity
    glm::vec3 m_vel = glm::vec3(0.f, 0.f, 0.f);

    yes_no m_immortal = NO;

    //in seconds
    float m_life;

    inline explicit particle(float life) :m_immortal(NO), m_life(life) {}
    inline explicit particle() :m_immortal(NO), m_life(10.f) {}

    void update_position()
    {
        float delta_time = jule::delta_time;

        m_xyz.x += m_vel.x * delta_time;
        m_xyz.y += m_vel.y * delta_time;
        m_xyz.z += m_vel.z * delta_time;

        m_vel.x += m_acc.x * delta_time;
        m_vel.y += m_acc.y * delta_time;
        m_vel.z += m_acc.z * delta_time;
    }
};

struct particle_system
{
    particle *m_prototype;
    size_t m_size;
    size_t m_bulk;
    float m_rate;
    glm::vec3 (*m_get_spawn_position)();
    glm::vec3 (*m_get_initial_acceleration)();
    glm::vec3 (*m_get_initial_velocity)();

    particle *particles;
    //3f position, 1f life
    float *particles_data; //[size*(3/*xyz*/+1/*life*/)];

    particle_system(
        particle *prototype,
        float rate_in_seconds,
        size_t bulk,
        glm::vec3 (*spawn_position)(),
        glm::vec3 (*acc_func_ptr)(),
        glm::vec3 (*vel_func_ptr)());


    void update_particles(glm::vec3 camera_position);

    inline void render_particles()
    {
        glDrawArrays(GL_POINTS, 0, particles_alive);
    }

    template<ALIVE A>
    void reset_particle(particle *p) const
    {
        if(A == alive)
            p->m_life = m_prototype->m_life;
        else
            p->m_life = -1.f;

        p->m_xyz = m_get_spawn_position();
        p->m_acc = m_get_initial_acceleration();
        p->m_vel = m_get_initial_velocity();
    }

    void reset_first_unused();
    
    inline void use()
    {
        glBindVertexArray(vao);
    }

private:
    unsigned int particles_alive = 0;
    unsigned int vao;
    unsigned int vbo;
};
#endif