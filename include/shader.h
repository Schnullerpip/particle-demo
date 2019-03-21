#ifndef jule_SHADER_H
#define jule_SHADER_H

#include"main.h"
#include"texture.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader;

namespace {
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }

    unsigned int createShader(int gl_shader_type, const char *code_source_path, const char *notation){
        std::string shader_code;
        std::ifstream shader_file;

        shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            shader_file.open(code_source_path);
            std::stringstream shader_stream;

            shader_stream << shader_file.rdbuf();
            shader_file.close();
            shader_code = shader_stream.str();
        }
        catch(std::ifstream::failure f)
        {
            std::cout << "ERROR::SHADER::" << notation << "::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        const char *shader_code_cstr = shader_code.c_str();

        unsigned int shader;

        shader = glCreateShader(gl_shader_type);
        glShaderSource(shader, 1, &shader_code_cstr, NULL);
        glCompileShader(shader);
        checkCompileErrors(shader, notation);

        return shader;
    }
}

namespace jule{
Shader * get_currently_bound_shader();
void set_currently_bound_shader(Shader *s);
}//namespace jule

class Shader
{
public:
    unsigned int ID;
    int pad[4];
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath = nullptr)
    {
        unsigned int vertex, fragment, geometry;

        vertex = createShader(GL_VERTEX_SHADER, vertexPath, "VERTEX");
        fragment = createShader(GL_FRAGMENT_SHADER, fragmentPath, "FRAGMENT");
        geometry = (geometryPath ? createShader(GL_GEOMETRY_SHADER, geometryPath, "GEOMETRY") : 0);


        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        if(geometryPath)
            glAttachShader(ID, geometry);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if(geometryPath)
            glDeleteShader(geometry);
    }
    // activate the shader
    // ------------------------------------------------------------------------
    void use() 
    { 
        jule::set_currently_bound_shader(this);
        glUseProgram(ID); 
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }

    void set_texture_uniform_data(Texture *texture)
    {
        texture->use();
        setInt(texture->get_name(), texture->get_nr());
    }

    void setMatrix4f(const std::string &name, glm::mat4 *m4)
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(*m4));
    }
    void setMatrix4f(const std::string &name, glm::mat4 m4)
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(m4));
    }
};
#endif