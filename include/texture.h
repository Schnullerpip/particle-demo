#ifndef jule_texture_h
#define  jule_texture_h

#include"stb_image.h"
#include"main.h"

class Texture
{
    const int width = 0, height = 0, nr_channels = 0;
    const unsigned int gl_texture_nr;
    const char *path;
    const char *uniform_name;
    const unsigned int ID = 0;

public:
    unsigned int get_id() const
    {
        return ID;
    }

    const char * get_name() const
    {
        return uniform_name;
    }

    unsigned int get_nr() const
    {
        return gl_texture_nr;
    }

    Texture(
        const char *path_to_file,
        const char *name,
        unsigned int gl_rgb_mode = GL_RGBA,
        unsigned int gl_texture_nr = 0)

        :gl_texture_nr(gl_texture_nr), uniform_name(name)
    {
        glGenTextures(1, const_cast<unsigned int*>(&ID));
        glActiveTexture(GL_TEXTURE0 + gl_texture_nr);
        glBindTexture(GL_TEXTURE_2D, ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        unsigned char *data = stbi_load(path_to_file, const_cast<int*>(&width), const_cast<int*>(&height), const_cast<int*>(&nr_channels), 0);

        if(data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, gl_rgb_mode, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            printf("failed to load texture\n");
        }
        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void use() const
    {
        glActiveTexture(GL_TEXTURE0 + gl_texture_nr);
        glBindTexture(GL_TEXTURE_2D, ID);
    }
};

#endif