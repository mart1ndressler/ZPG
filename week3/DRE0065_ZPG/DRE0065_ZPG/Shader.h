#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <string>

using namespace std;

class Shader
{
    public:
        Shader(const char* filePath, GLenum shaderType);
        ~Shader();
        GLuint shaderID() const {return id;}

    private:
        GLuint id = 0;
        string readTextFile(const char* path);
        void checkShaderCompilation(GLuint shader);
};
#endif