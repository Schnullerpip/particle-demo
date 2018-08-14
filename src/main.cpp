#include"main.h"
#include"input_management.h"
#include"setup.h"
#include"shader.h"

Shader basic("shader/vertex/basic.vs", "shader/fragment/basic.fs");

int main(){
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    unsigned int vao, vbo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    basic.use();

    while(!glfwWindowShouldClose(jule::window))
    {
        // input handling
        jule::processInput(jule::window);

        // rendering commands 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(jule::window);
    }

    glfwTerminate();
    return 0;
}