#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h>
#include <cstdio>
#include <cstdlib>

#include "Shader.h"

class ShaderProgram
{
    public:
        ShaderProgram(const char* vertexSrc, const char* fragmentSrc);
        ~ShaderProgram();
        void use() const;

    private:
        GLuint programID = 0;
        void checkLinking(GLuint program);
};
#endif