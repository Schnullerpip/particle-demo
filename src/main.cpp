#include<cmath>
#include<chrono>

#include"main.h"
#include"input_management.h"
#include"setup.h"
#include"shader.h"
#include"texture.h"
#include"camera.h"
#include"particle.h"
#include"debug_line.h"

//#define debug
//#define fire
#define canon

struct measurements {
    measurements(unsigned max_frames);
    ~measurements();
    void start();
    void end();
    void update();

    std::vector<float> delta_times;
    std::chrono::steady_clock::time_point start_time_stamp;
    std::chrono::steady_clock::time_point end_time_stamp;
};
measurements::measurements(unsigned max_frames)
{
    delta_times.reserve(max_frames > 0 ? max_frames : 10000);
}
void measurements::update()
{
    delta_times.push_back(jule::delta_time);
}
void measurements::start()
{
    start_time_stamp = std::chrono::steady_clock::now();
}
void measurements::end()
{
    end_time_stamp = std::chrono::steady_clock::now();
    std::time_t t = std::time(0);
    std::tm *now = std::localtime(&t);

    std::stringstream ss;
    ss << "delta_times_over_" << delta_times.size() << "_frames_" <<
        now->tm_hour << ":" << now->tm_min << ".csv";

    std::string file_name = ss.str();
    std::ofstream file;
    file.open(file_name);
    file << "frame#delta_times";
    float delta_time_avg = 0;
    for(unsigned i = 0; i < delta_times.size(); ++i){
        delta_time_avg += delta_times[i];
        file << "\n" << i << "#" << delta_times[i];
    }
    delta_time_avg /= delta_times.size();

    std::cout << "measured range: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time_stamp - start_time_stamp).count()
              << " ms\ndelta time avg: " << delta_time_avg << "\nframes: " << delta_times.size();

    file.close();
}
measurements::~measurements()
{
}

namespace jule
{
    float delta_time = 0.0f;
    std::random_device rd;
    std::mt19937 gen(rd());

    bool running = true;
    float EPSILON = 0.000005f;
}

void print_vec(glm::vec3 &&v)
{
    printf("(%f,%f,%f)", v.x, v.y, v.z);
}
void print_vec(glm::vec3 &v)
{
    printf("(%f,%f,%f)", v.x, v.y, v.z);
}

int main(){
    printf("sizeof particle: %lu\n", sizeof(particle));
    srand(static_cast<unsigned>(time(0)));


    jule::InputManager *input_manager = jule::InputManager::get_instance();

    Camera cam;

    input_manager->EventSource<WASD_key_input>::add_listener(&cam);
    input_manager->EventSource<mouse_input>::add_listener(&cam);
    input_manager->EventSource<pause>::add_delegate([](pause p){jule::running = !jule::running;});

    Shader basic("shader/vertex/basic.vs", "shader/fragment/basic.fs");
    Shader fire_particle_shader("shader/vertex/particle.vs", "shader/fragment/fire_particle.fs", "shader/geometry/particle.gs");
    Shader collision_particle_shader("shader/vertex/particle.vs", "shader/fragment/particle.fs", "shader/geometry/basic.gs");
    Shader line_shader("shader/vertex/line.vs", "shader/fragment/line.fs");

    Texture wood("img/container.jpg", "u_texture_1", GL_RGB, 0);
    Texture face("img/comicface.png", "u_texture_2", GL_RGBA, 1);

    Texture particle_tex("img/particle.png", "u_texture_1", GL_RGB, 0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //vs
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //triangle
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int vao, vbo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*180, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(GLfloat)*3));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    glm::vec3 cubePositions[10] = {
        glm::vec3( 0.0f,  0.0f,  -7.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    glBindVertexArray(vao);


    //PARTICLES

#ifdef fire
    const size_t num_particles = 1000;
    particle proto_particle(1.f, &fire_particle_shader, &particle_tex);
    proto_particle.m_radius = .3f;

    particle_system particle_sys(proto_particle, num_particles, 25, .04f, NO, NO,
    [](){
        static glm::vec3 axis = glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 rotation;
        rotation = glm::rotate(rotation, 0.f, axis);
        glm::vec4 pos = glm::vec4(
          jule::get_rnd_float(-0.031f, 0.031f),
          jule::get_rnd_float(-0.031f, 0.031f),
          jule::get_rnd_float(-0.031f, 0.031f),
          1.f) * rotation;
        return glm::vec3(pos.x, pos.y, pos.z);
    },
    [](){
        glm::vec3 acc(
            0.f,
            jule::get_rnd_float(2.7f,  3.3f),
            0.f
        );
        //direction = glm::normalize(direction);
        return acc;
        //return glm::vec3(0.f, 0.f, 0.f);
    },
    [](){
        float vel_x = jule::get_rnd_float(0.0f, 2.8f) * (jule::get_rnd_bool() ? -1.f : 1.f);
        float vel_y = jule::get_rnd_float(-2.f, 2.f);
        float vel_z = jule::get_rnd_float(0.0f, 2.8f) * (jule::get_rnd_bool() ? -1.f : 1.f);
        return glm::normalize(glm::vec3(vel_x, vel_y, vel_z)) * .6f;
    });
#endif

#ifdef canon
    constexpr float offset = 5.f;
    constexpr float spawn_radius = 1.f;

    constexpr float vel_factor= 5.f;
    constexpr int num_particles_per_gun = 100000;
    constexpr int particle_bulk_per_gun = 30;
    constexpr float gun_shoot_rate = 0.01f;

    constexpr yes_no internal_col_pol = NO;
    constexpr yes_no global_col_pol = YES;

    particle collision_proto_particle(4.f, &collision_particle_shader, &particle_tex);
    collision_proto_particle.m_radius = .4f;

    //particle CANON 1
    particle_system collision_system1(collision_proto_particle, num_particles_per_gun, particle_bulk_per_gun, gun_shoot_rate, internal_col_pol, global_col_pol,
    [](){
        static glm::vec3 spawn_center(0.f, 0.f, -offset);
        static glm::vec3 spawn_tilt_x_axis = glm::normalize(glm::vec3(0.f, 1.f, -1.f));
        static glm::vec3 spawn_tilt_z_axis = glm::vec3(1.f, 0.f, 0.f);

        glm::vec3 spawn_pos(jule::get_rnd_float(-1.f, 1.f), jule::get_rnd_float(-1.f, 1.f), 0.f);
        glm::vec3 x_tilted = jule::project_on(spawn_pos, spawn_tilt_x_axis);
        glm::vec3 z_tilted = jule::project_on(spawn_pos, spawn_tilt_z_axis);
        spawn_pos = glm::normalize(x_tilted + z_tilted) * jule::get_rnd_float(0.f, spawn_radius);
        return spawn_center + spawn_pos;
    },
    [](){return glm::vec3(0.f, 0.f, 0.f);},
    [](){return glm::vec3(0.f, vel_factor, vel_factor);});

    //particle CANON 2
    memcpy(collision_proto_particle.m_color, jule::BLUE, 3 * sizeof(float));
    particle_system collision_system2(collision_proto_particle, num_particles_per_gun, particle_bulk_per_gun, gun_shoot_rate, internal_col_pol, global_col_pol,
    [](){
        static glm::vec3 spawn_center(0.f, 0.f, offset);
        static glm::vec3 spawn_tilt_x_axis = glm::normalize(glm::vec3(0.f, 1.f, 1.f));
        static glm::vec3 spawn_tilt_z_axis = glm::vec3(1.f, 0.f, 0.f);

        glm::vec3 spawn_pos(jule::get_rnd_float(-1.f, 1.f), jule::get_rnd_float(-1.f, 1.f), 0.f);
        glm::vec3 x_tilted = jule::project_on(spawn_pos, spawn_tilt_x_axis);
        glm::vec3 z_tilted = jule::project_on(spawn_pos, spawn_tilt_z_axis);
        spawn_pos = glm::normalize(x_tilted + z_tilted) * jule::get_rnd_float(0.f, spawn_radius);
        return spawn_center + spawn_pos;
    },
    [](){return glm::vec3(0.f, 0.f, 0.f);},
    [](){return glm::vec3(0.f, vel_factor, -vel_factor);});

    //particle CANON 3
    memcpy(collision_proto_particle.m_color, jule::RED, 3 * sizeof(float));
    particle_system collision_system3(collision_proto_particle, num_particles_per_gun, particle_bulk_per_gun, gun_shoot_rate, internal_col_pol, global_col_pol,
    [](){
        static glm::vec3 spawn_center(offset, 0.f, 0.f);
        static glm::vec3 spawn_tilt_z_axis = glm::normalize(glm::vec3(1.f, 1.f, 0.f));
        static glm::vec3 spawn_tilt_x_axis = glm::vec3(0.f, 0.f, 1.f);

        glm::vec3 spawn_pos(jule::get_rnd_float(-1.f, 1.f), 0.f, jule::get_rnd_float(-1.f, 1.f));
        glm::vec3 z_tilted = jule::project_on(spawn_pos, spawn_tilt_z_axis);
        glm::vec3 x_tilted = jule::project_on(spawn_pos, spawn_tilt_x_axis);
        spawn_pos = glm::normalize(x_tilted + z_tilted) * jule::get_rnd_float(0.f, spawn_radius);
        return spawn_center + spawn_pos;
    },
    [](){return glm::vec3(0.f, 0.f, 0.f);},
    [](){return glm::vec3(-vel_factor, vel_factor, 0.f);});

    //particle CANON 4
    memcpy(collision_proto_particle.m_color, jule::GREEN, 3 * sizeof(float));
    collision_proto_particle.m_radius = 2.2f;
    collision_proto_particle.m_mass = 9000.f;
    particle_system collision_system4(collision_proto_particle, num_particles_per_gun, 1, 1.f, internal_col_pol, global_col_pol,
    [](){
        static glm::vec3 spawn_center(-offset, 0.f, 0.f);
        static glm::vec3 spawn_tilt_z_axis = glm::normalize(glm::vec3(-1.f, 1.f, 0.f));
        static glm::vec3 spawn_tilt_x_axis = glm::vec3(0.f, 0.f, 1.f);

        glm::vec3 spawn_pos(jule::get_rnd_float(-1.f, 1.f), 0.f, jule::get_rnd_float(-1.f, 1.f));
        glm::vec3 z_tilted = jule::project_on(spawn_pos, spawn_tilt_z_axis);
        glm::vec3 x_tilted = jule::project_on(spawn_pos, spawn_tilt_x_axis);
        spawn_pos = glm::normalize(x_tilted + z_tilted) * jule::get_rnd_float(0.f, spawn_radius);
        return spawn_center + spawn_pos;
    },
    [](){return glm::vec3(0.f, 0.f, 0.f);},
    [](){return glm::vec3(vel_factor*4, vel_factor*4, 0.f);});
#endif 

//GAMELOOP
{
    float last_frame = 0.0f;
    jule::running = true;

    unsigned count = 0;
    constexpr unsigned max = 200;
    measurements measure(max);
    measure.start();
    while(!glfwWindowShouldClose(jule::window) && (count++ < max))
    {
        float current_frame = glfwGetTime();
        jule::delta_time = current_frame - last_frame;
        last_frame = current_frame;

        measure.update();

        // rendering commands 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // input handling
        jule::processInput(jule::window);


        //transformations
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(jule::window_width) / jule::window_height, 0.1f, 700.f);

        glm::mat4 view = cam.update();

        basic.use();
        //texture usage
        basic.set_texture_uniform_data(&wood);
        basic.set_texture_uniform_data(&face);
        //transform usage
        basic.setMatrix4f("u_projection", &projection);
        basic.setMatrix4f("u_view", &view);

        glBindVertexArray(vao);
        for(unsigned int i = 0; i < 10; ++i)
        {
            glm::mat4 model;
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, 20.f * i, glm::vec3(1.0f, 0.3f, 0.5f));
            basic.setMatrix4f("u_model", &model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //PARTICLES
        particle_system::Update();
        for(auto ps : particle_system::particle_systems)
        {
            particle &prototype = ps->m_prototype;
            Shader *s = prototype.m_particle_shader;

            ps->use();
            //life info
            s->setFloat("max_life", prototype.m_life);
            //radius info
            s->setFloat("particle_radius", prototype.m_radius);
            //texture usage
            s->set_texture_uniform_data(prototype.m_particle_texture);
            //transform usage
            s->setMatrix4f("u_projection", &projection);
            s->setMatrix4f("u_view", &view);
            ps->render_particles(cam.getPosition());

#ifdef debug
            for(unsigned i = 0; i < ps->NumberParticlesAlive(); ++i)
            {
                particle &p = ps->particles[i];
                glm::vec3 v1 = p.m_xyz;
                glm::vec3 v2 = v1;

                v1.z += 0.1f;
                v2.x += p.m_radius;
                v2.z += 0.1f;

                draw_line(v1, v2, line_shader, view, projection);
            }
#endif
        }
        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(jule::window);
    }
    measure.end();
}

    glfwTerminate();
    return 0;
}

namespace jule{
float get_rnd_float(float min, float max)
{
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}
bool get_rnd_bool()
{
    std::uniform_int_distribution<> dis(0, 1);
    return dis(gen) == 0 ? true : false;
}

int get_rnd_int(int min, int max)
{
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

glm::vec3 rotateAround(glm::vec3 &pointToRotate, glm::vec3 &rotationCenter, glm::mat4 &rotation)
{
    glm::vec4 pointToRotate_ = glm::vec4(pointToRotate, 1.f);

    glm::mat4 translation = glm::translate(glm::mat4(), rotationCenter);
    glm::mat4 inv_translation = glm::inverse(translation);

    glm::mat4 transform = translation * rotation * inv_translation;

    glm::vec4 a = transform * pointToRotate_;
    glm::vec3 rotated_point(a.x, a.y, a.z);
    return rotated_point;
}

glm::vec3 project_on(glm::vec3 v1, glm::vec3 v2)
{
    v2 = glm::normalize(v2);
    return v2 * (glm::dot(v1, v2));
}
}//namespace jule
