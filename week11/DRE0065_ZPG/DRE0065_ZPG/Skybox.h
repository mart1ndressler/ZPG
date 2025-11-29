#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include "ShaderProgram.h"

using namespace std;
using namespace glm;

class Skybox
{
    public:
        Skybox(const char* posx, const char* negx, const char* posy, const char* negy, const char* posz, const char* negz);
        ~Skybox();
        void applySB(ShaderProgram* shaderProgram, bool asBackground, const mat4* modelMatrix = nullptr, int textureUnit = 5);

    private:
        GLuint VAO = 0, VBO = 0, cubemap = 0;
};