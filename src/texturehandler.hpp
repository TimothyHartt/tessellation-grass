#pragma once

class TextureHandler
{
private:
    static TextureHandler* instance;

public:

    TextureHandler() = default;
    static TextureHandler* getInstance();
    unsigned int GenTexture(const char* path);

};

