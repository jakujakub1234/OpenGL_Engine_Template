/*
The code template was taken from https://learnopengl.com/
*/

#pragma once

#include <vector>

class TextureManager
{
public:
    unsigned int LoadTexture(char* file_name, bool is_png);
    unsigned int LoadCubemap(std::vector<char*> faces);
};
