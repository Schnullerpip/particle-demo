#include"particle.h"
#include"collision_detection.h"
#include"main.h"

std::vector<particle_system*> particle_system::particle_systems = {};


void collide(particle &p1, particle &p2, collision_data &collision_data)
{
    // First, find the normalized vector n from the center of 
    // circle1 to the center of circle2
    glm::vec3 n = p1.m_xyz - p2.m_xyz;
    n = glm::normalize(n);
    // Find the length of the component of each of the movement
    // vectors along n. 
    // a1 = v1 . n
    // a2 = v2 . n
    float a1 = glm::dot(p1.m_vel, n);
    float a2 = glm::dot(p2.m_vel, n);

    // Using the optimized version,     
    // optimizedP =  2(a1 - a2)
    //              -----------
    //                m1 + m2
    float optimizedP = (2.0 * (a1 - a2)) / (p1.m_mass + p2.m_mass);

    // Calculate v1', the new movement vector of circle1
    // v1' = v1 - optimizedP * m2 * n
    glm::vec3 new_vel_p1 = p1.m_vel - optimizedP * p2.m_mass * n;

    // Calculate v1', the new movement vector of circle1
    // v2' = v2 + optimizedP * m1 * n
    glm::vec3 new_vel_p2 = p2.m_vel + optimizedP * p1.m_mass * n;

    p1.m_vel =new_vel_p1;
    p2.m_vel =new_vel_p2;

    // the particles are probably inside each other - move them back accordingly
    //or better check which particle has the greater force and then move the weak one!
    float p1_force = glm::length(p1.m_vel) * p1.m_mass;
    float p2_force = glm::length(p2.m_vel) * p2.m_mass;

    float correction = collision_data.merge_distance;
    if(p1_force < p2_force)
    {
        p1.m_xyz += collision_data.dir_vec_from_p2_to_collision_point *
                    (correction + jule::EPSILON);
    }else{
        p2.m_xyz += collision_data.dir_vec_from_p1_to_collision_point *
                    (correction + jule::EPSILON);
    }
}


//should only be called after update_particles because we try to only calculate the particles 
void particle_system::particle_system_collision(particle_system *ps1, particle_system *ps2)
{
    for(unsigned i = 0; i < ps1->particles_alive; ++i)
    {
        particle *p1 = ps1->particles[i];
        for(unsigned o = (ps1 == ps2 ? i+1 : 0); o < ps2->particles_alive; ++o)
        {
            particle *p2 = ps2->particles[o];
            auto collision_data = jule::sphere_collision(p1->m_xyz, p1->m_radius, p2->m_xyz, p2->m_radius);
            if(collision_data.colliding)
            {
                collide(*p1, *p2, collision_data);
            }
        }
    }
}

void particle_system::GlobalParticleCollisions()
{
    for(unsigned i = 0; i < particle_systems.size(); ++i)
    {
        particle_system *ps1 = particle_systems[i];
        if(ps1->m_global_particle_collision == NO)
            continue;
        unsigned o = i+1;
        for(; o < particle_systems.size(); ++o)
        {
            particle_system *ps2 = particle_systems[o];
            if(ps2->m_global_particle_collision == NO)
                continue;
            particle_system_collision(ps1, particle_systems[o]);
        }
    }
}

void particle_system::Update()
{
    if(!jule::running){
        return;
    }

    for(auto ps : particle_system::particle_systems)
    {
        ps->update_particles();
    }

    //collision handling
    particle_system::GlobalParticleCollisions();

    for(auto ps : particle_system::particle_systems)
    {
        ps->intern_particle_collision();
    }
}

constexpr size_t stride = 3/*xyz*/+1/*life*/+3/*color*/;

particle_system::particle_system(
    particle prototype,
    size_t size,
    size_t bulk,
    float rate_in_seconds,
    yes_no intern_particle_collision,
    yes_no global_particle_collision,
    glm::vec3 (*spawn_position)(),
    glm::vec3 (*acc_func_ptr)(),
    glm::vec3 (*vel_func_ptr)()):
    m_prototype(prototype),
    m_size(size),
    m_bulk(bulk),
    m_rate(rate_in_seconds),
    m_intern_particle_collision(intern_particle_collision),
    m_global_particle_collision(global_particle_collision),
    m_get_spawn_position(spawn_position),
    m_get_initial_acceleration(acc_func_ptr),
    m_get_initial_velocity(vel_func_ptr)
{

    particles = new particle*[m_size];
    for(size_t i = 0; i < m_size; ++i)
    {
        //TODO make this worse by having an array of particle* and getting each individual with new
        particles[i] = new particle(m_prototype);
        reset_particle(particles[i], dead);
    }

    particles_data = new float[m_size*(stride)];

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * stride * m_size, particles_data, GL_DYNAMIC_DRAW);

    GLsizei stride_size = stride * sizeof(GLfloat);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride_size, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, stride_size, (void*)(sizeof(GLfloat)*3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride_size, (void*)(sizeof(GLfloat)*4));
    glEnableVertexAttribArray(2);

    //register to the container, that holds all particle_systems
    particle_systems.push_back(this);
}

void particle_system::intern_particle_collision()
{
    if(m_intern_particle_collision == YES)
    particle_system_collision(this, this);
}
void particle_system::update_particles()
{
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

    for(size_t s = 0; s < m_size; ++s)
    {
        particle *p = particles[s];

        p->update_position();

        if((p->m_life -= delta_time) > 0.f)
        {
            ++particles_alive;
        }
    }

    //sort out dead particles
    std::sort(particles, particles+m_size, [](particle *p1, particle *p2){
        return p1->m_life > p2->m_life;});

}

void particle_system::render_particles(glm::vec3 camera_position)
{
    //sort alive particles in correct order for camera (so blending is right)
    std::sort(particles, particles+particles_alive, [&](particle *p1, particle *p2){
        return  glm::length2(camera_position-p1->m_xyz) > glm::length2(camera_position-p2->m_xyz);
    });

    for(size_t s = 0; s < particles_alive; ++s)
    {
        particle *p = particles[s];

        size_t offset = s * stride;
        particles_data[offset + x] = p->m_xyz.x;
        particles_data[offset + y] = p->m_xyz.y;
        particles_data[offset + z] = p->m_xyz.z;
        particles_data[offset + life] = p->m_life;
        particles_data[offset + r] = p->m_color[0];
        particles_data[offset + g] = p->m_color[1];
        particles_data[offset + b] = p->m_color[2];
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * stride * particles_alive, particles_data, GL_DYNAMIC_DRAW);

    glDrawArrays(GL_POINTS, 0, particles_alive);
}

void particle_system::reset_particle(particle *p, ALIVE a) const 
{
    *p = m_prototype;

    if(a == alive)
        p->m_life = m_prototype.m_life;
    else
        p->m_life = -1.f;

    p->m_xyz = m_get_spawn_position();
    p->m_acc = m_get_initial_acceleration();
    p->m_vel = m_get_initial_velocity();
    memcpy(p->m_color, m_prototype.m_color, 3*sizeof(float));
}

void particle_system::reset_first_unused()
{
    static unsigned int last_found = 0;
    for(unsigned int i = last_found; i < m_size; ++i)
    {
        particle &p = *particles[i];
        if((p.m_immortal == NO) && (p.m_life < 0.f))
        {
            last_found = i;
            reset_particle(&p, alive);
            return;
        }
    }
    for(unsigned int i = 0; i < last_found; ++i)
    {
        particle &p = *particles[i];
        if((p.m_immortal == NO) && (p.m_life < 0.f))
        {
            last_found = i;
            reset_particle(&p, alive);
            return;
        }
    }
}

particle_system::~particle_system()
{
    for(size_t i = 0; i < m_size; ++i)
    {
        delete particles[i];
    }
    delete[] particles;
    delete particles_data;
}
