#include<cmath>

#include"main.h"
#include"input_management.h"
#include"setup.h"
#include"shader.h"
#include"texture.h"
#include"camera.h"
#include"particle.h"

namespace jule
{
    float delta_time = 0.0f;
    std::random_device rd;
    std::mt19937 gen(rd());
}

int main(){
    srand(static_cast<unsigned>(time(0)));


    jule::InputManager *input_manager = jule::InputManager::get_instance();

    Camera cam;

    input_manager->EventSource<WASD_key_input>::add_listener(&cam);
    input_manager->EventSource<mouse_input>::add_listener(&cam);

    Shader basic("shader/vertex/basic.vs", "shader/fragment/basic.fs");
    Shader particle_shader("shader/vertex/particle.vs", "shader/fragment/particle.fs", "shader/geometry/particle.gs");

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
        glm::vec3( 3.0f,  0.0f,  0.0f), 
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

    const size_t num_particles = 100;
    particle proto_particle(1.f);

    particle_system<num_particles> particle_sys(&proto_particle, .53f, 15,
    [](){
        static glm::vec3 axis = glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 rotation;
        rotation = glm::rotate(rotation, 90.f, axis);
        glm::vec4 pos = glm::vec4(
          jule::get_rnd_float(-0.2f, 0.2f),
          jule::get_rnd_float(0.f, 0.f),
          jule::get_rnd_float(-0.2f, 0.2f),
          1.f) * rotation;
        return glm::vec3(pos.x, pos.y, pos.z);

        //return glm::vec3(0.f, 0.f, 0.f);
    },
    [](){
        glm::vec3 direction(
            //jule::get_rnd_float(0.f, 0.f),
            //jule::get_rnd_float(0.f, 0.f),
            //jule::get_rnd_float(0.f, 0.f)
            0.f, 0.f, 0.f
        );
        direction = glm::normalize(direction);
        return glm::vec3(0.f, 0.f, 0.f);
    },
    [](){
        return glm::vec3(0.f, 0.f, 0.f);
    });


//GAMELOOP
{
    float last_frame = 0.0f;

    while(!glfwWindowShouldClose(jule::window))
    {
        float current_frame = glfwGetTime();
        jule::delta_time = current_frame - last_frame;
        last_frame = current_frame;

        // rendering commands 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // input handling
        jule::processInput(jule::window);


        //transformations
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(jule::window_width) / jule::window_height, 0.1f, 100.f);

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




        {//PARTICLES
            particle_shader.use();
            //texture usage
            particle_shader.set_texture_uniform_data(&particle_tex);
            //transform usage
            particle_shader.setMatrix4f("u_projection", &projection);
            particle_shader.setMatrix4f("u_view", &view);

            particle_sys.use();
            particle_sys.update_particles(cam.getPosition());
            particle_sys.render_particles();
        }

        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(jule::window);
    }
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
}//namespace jule
