#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <iostream>
#include <stdexcept>
#include <stb_image.h>

#include "Shader.h"

class Texture
{
public:
    //  Texture program ID
    unsigned int ID;

    //  Constructor
    Texture(char const *image, GLenum format)
    {
        //  Generate and bind texture
        glGenTextures(1, &ID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ID);

        //  Set texture wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        //  Set texture filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //  Load image
        int width, height, nrChannels;
        const int requestedChannels = format == GL_RGBA ? 4 : 3;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *bytes = stbi_load(image, &width, &height, &nrChannels, requestedChannels);
        if (bytes)
        {
            GLint oldAlignment;
            glGetIntegerv(GL_UNPACK_ALIGNMENT, &oldAlignment);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE,
                         bytes);
            glPixelStorei(GL_UNPACK_ALIGNMENT, oldAlignment);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            glDeleteTextures(1, &ID);
            ID = 0;
            throw std::runtime_error(std::string("Failed to load texture: ") + image);
        }
        //  Load image

        //  Free memory from image data and unbind texture
        stbi_image_free(bytes);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    ~Texture() { Delete(); }

    //  Create a texture uniform
    void texUnit(Shader &shader, const char *uniform)
    {
        GLuint texUni = glGetUniformLocation(shader.ID, uniform);
        shader.Activate();
        glUniform1i(texUni, 0);
    }

    //  Bind texture
    void Bind() { glBindTexture(GL_TEXTURE_2D, ID); }

    //  Delete texture
    void Delete() { if (ID) { glDeleteTextures(1, &ID); ID = 0; } }
};
#endif// !TEXTURE_H
