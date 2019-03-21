#ifndef jule_particle_h
#define  jule_particle_h

#include<algorithm>
#include<glm/gtx/norm.hpp>

#include"main.h"
#include"shader.h"

enum yes_no {YES, NO};
enum ALIVE {alive, dead};
enum PARTICLE_ATTRIB {x, y, z, life, r, g, b};

struct particle
{
    glm::vec3 m_xyz = glm::vec3(0.f, 0.f, 0.f);
    //int pad[100];
    Shader *m_particle_shader = nullptr;
    Texture m_particle_texture;
    glm::vec3 m_acc = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 m_vel = glm::vec3(0.f, 0.f, 0.f);
    yes_no m_immortal = NO;
    float m_life;
    float m_radius = 1.f;
    float m_mass = 1.f;
    float m_color[3] = {1.f, 0.6f, 0.3f};

    particle(float life, Shader *shader, Texture *texture)
        :m_immortal(NO),
        m_life(life)
        {
            m_particle_shader = shader;
            m_particle_texture = *texture;
        }
    particle()
        :m_immortal(NO),
        m_life(10.f)
        {}

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
    //container with all particle systems - particle systems will register to it at construction
    static std::vector<particle_system*> particle_systems;
    static void GlobalParticleCollisions();
    static void Update();
    static void particle_system_collision(particle_system *ps1, particle_system *ps2);

    particle m_prototype;
    size_t m_size;
    size_t m_bulk;
    float m_rate;
    yes_no m_intern_particle_collision;
    yes_no m_global_particle_collision;
    glm::vec3 (*m_get_spawn_position)();
    glm::vec3 (*m_get_initial_acceleration)();
    glm::vec3 (*m_get_initial_velocity)();

    particle **particles;
    //3f position, 1f life
    float *particles_data; //[size*(3/*xyz*/+1/*life*/+3/*color*/)];

    particle_system(
        particle prototype,
        size_t size,
        size_t bulk,
        float rate_in_seconds,
        yes_no intern_particle_collision,
        yes_no global_particle_collision,
        glm::vec3 (*spawn_position)(),
        glm::vec3 (*acc_func_ptr)(),
        glm::vec3 (*vel_func_ptr)());

    void intern_particle_collision();
    void update_particles();

    void render_particles(glm::vec3 camera_position);

    void reset_particle(particle *p, ALIVE a) const;

    void reset_first_unused();
    
    void use()
    {
        m_prototype.m_particle_shader->use();
        glBindVertexArray(vao);
    }

    inline unsigned int NumberParticlesAlive()const {return particles_alive;}

    ~particle_system();

private:
    unsigned int particles_alive = 0;
    unsigned int vao;
    unsigned int vbo;
    float time_count = 0;
};
#endif