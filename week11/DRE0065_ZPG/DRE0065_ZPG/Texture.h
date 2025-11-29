#pragma once
#include <GL/glew.h>
#include <cstdio>
#include <string>
#include "ShaderProgram.h"

using namespace std;

class Texture
{
    public:
        Texture(const char* fileName);
        ~Texture();
        void applyT(ShaderProgram* shaderProgram, int unit, int useTexture = 1, float uvTiling = 1.0f) const;

    private:
        GLuint tex = 0;
};