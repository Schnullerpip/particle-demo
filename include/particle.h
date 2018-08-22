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

template<unsigned int size>
struct particle_system
{
    particle *m_prototype;
    float m_rate;
    size_t m_bulk;
    glm::vec3 (*m_get_spawn_position)();
    glm::vec3 (*m_get_initial_acceleration)();
    glm::vec3 (*m_get_initial_velocity)();

    particle particles[size];
    //3f position, 1f life
    float particles_data[size*(3/*xyz*/+1/*life*/)];

    particle_system(particle *prototype, float rate_in_seconds, size_t bulk, glm::vec3 (*spawn_position)(), glm::vec3 (*acc_func_ptr)(), glm::vec3 (*vel_func_ptr)())
        :m_prototype(prototype),
        m_rate(rate_in_seconds),
        m_bulk(bulk),
        m_get_spawn_position(spawn_position),
        m_get_initial_acceleration(acc_func_ptr),
        m_get_initial_velocity(vel_func_ptr)
    {
        for(size_t i = 0; i < size; ++i)
        {
            reset_particle<dead>(&particles[i]);
        }

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * size, particles_data, GL_DYNAMIC_DRAW);

        GLsizei stride = 4 * sizeof(GLfloat);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(GLfloat)*3));
        glEnableVertexAttribArray(1);
    }

    unsigned int particles_alive = 0;
    void update_particles(glm::vec3 camera_position)
    {
        static float time_count = 0;
        float delta_time = jule::delta_time;
        particles_alive = 0;

        if((time_count += delta_time) >= m_rate)
        {
            time_count = 0;
            for(size_t i = 0; i < m_bulk; ++i)
            {
                reset_first_unused();
            }
        }

        for(size_t s = 0; s < size; ++s)
        {
            particle &p = particles[s];

            p.update_position();

            if((p.m_life -= delta_time) > 0.f)
            {
                ++particles_alive;
            }
        }

        //sort out dead particles
        std::sort(particles, particles+size, [](particle &p1, particle &p2){
            return p1.m_life > p2.m_life;});

        //sort alive particles in correct order for camera (so blending is right)
        std::sort(particles, particles+particles_alive, [&](particle &p1, particle &p2){
            return  glm::length2(camera_position-p1.m_xyz) > glm::length2(camera_position-p2.m_xyz);
        });

        for(size_t s = 0; s < particles_alive; ++s)
        {
            particle &p = particles[s];

            size_t offset = s * 4;
            particles_data[offset + x] = p.m_xyz.x;
            particles_data[offset + y] = p.m_xyz.y;
            particles_data[offset + z] = p.m_xyz.z;
            particles_data[offset + life] = p.m_life;
        }

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * particles_alive, particles_data + num_dead, GL_DYNAMIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * particles_alive, particles_data, GL_DYNAMIC_DRAW);
    }

    void render_particles()
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

    void reset_first_unused()
    {
        static unsigned int last_found = 0;
        for(unsigned int i = last_found; i < size; ++i)
        {
            particle &p = particles[i];
            if((p.m_immortal == NO) && (p.m_life < 0.f))
            {
                last_found = i;
                reset_particle<alive>(&p);
                return;
            }
        }
        for(unsigned int i = 0; i < last_found; ++i)
        {
            particle &p = particles[i];
            if((p.m_immortal == NO) && (p.m_life < 0.f))
            {
                last_found = i;
                reset_particle<alive>(&p);
                return;
            }
        }
    }
    
    void use()
    {
        glBindVertexArray(vao);
    }
private:
    unsigned int vao;
    unsigned int vbo;
};
#endif