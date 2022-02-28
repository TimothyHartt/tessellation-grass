#include "texturehandler.hpp"
#include <glad/glad.h>
#include "stb_image.h"
#include <iostream>

TextureHandler * TextureHandler::instance = nullptr;

TextureHandler * TextureHandler::getInstance()
{
    if (instance == nullptr)
    {
        instance = new TextureHandler();
    }
    return instance;
}

unsigned int TextureHandler::GenTexture(const char* path)
{
    unsigned int texture;
    glGenTextures(1, &texture);

    int width, height, channel_num;
    unsigned char *data = stbi_load(path, &width, &height, &channel_num, 0); 

    if (data)
    {
        GLenum format = GL_RED;
        if(channel_num == 1)
        {
            format = GL_RED;
        }
        else if(channel_num == 3)
        {
            format = GL_RGB;
        }
        else if(channel_num == 4)
        {
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    return texture;
}
    
