#ifndef JULE_DEBUG_LINE_H
#define JULE_DEBUG_LINE_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"shader.h"


void draw_line(glm::vec3 p1, glm::vec3 p2, Shader &s, glm::mat4 view, glm::mat4 projection)
{
    float points[6] = {p1.x, p1.y, p1.z,    p2.x, p2.y, p2.z};
    //float points[6] = {200, 100, 0,      100, 300, 0};

    glEnable(GL_LINE_SMOOTH);

    unsigned int vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6, points, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    s.use();
    s.setMatrix4f("u_view", view);
    s.setMatrix4f("u_projection", projection);

    glDrawArrays(GL_LINES, 0, 2);

    glBindVertexArray(0);
}

#endif