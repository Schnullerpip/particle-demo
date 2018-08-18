#include<cmath>

#include"main.h"
#include"input_management.h"
#include"setup.h"
#include"shader.h"
#include"texture.h"
#include"camera.h"

namespace jule
{
    float delta_time = 0.0f;
}

int main(){
    jule::InputManager *input_manager = jule::InputManager::get_instance();

    Camera cam;

    input_manager->EventSource<WASD_key_input>::add_listener(&cam);
    input_manager->EventSource<mouse_input>::add_listener(&cam);

    Shader basic("shader/vertex/basic.vs", "shader/fragment/basic.fs");

    Texture wood("img/container.jpg", "u_texture_1", GL_RGB, 0);
    Texture face("img/comicface.png", "u_texture_2", GL_RGBA, 1);


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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(GLfloat)*3));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    glm::vec3 cubePositions[10] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
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

    //shader usage
    basic.use();

    //texture usage
    basic.set_texture_uniform_data(&wood);
    basic.set_texture_uniform_data(&face);

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
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), static_cast<float>(jule::window_width / jule::window_height), 0.1f, 100.f);

        //glUniformMatrix4fv(glGetUniformLocation(basic.ID, "u_model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(basic.ID, "u_projection"), 1, GL_FALSE, glm::value_ptr(projection));

        for(unsigned int i = 0; i < 10; ++i)
        {
            glm::mat4 model;
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.f * i;
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(glGetUniformLocation(basic.ID, "u_model"), 1, GL_FALSE, glm::value_ptr(model));


            glUniformMatrix4fv(glGetUniformLocation(basic.ID, "u_view"), 1, GL_FALSE, glm::value_ptr(cam.update()));

            glDrawArrays(GL_TRIANGLES, 0, 36);
            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }


        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(jule::window);
    }
}

    delete input_manager;
    glfwTerminate();
    return 0;
}