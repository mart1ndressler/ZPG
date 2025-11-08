#pragma once
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
        GLuint shaderID() const;

    private:
        string readTextFile(const char* path);
        void checkShaderCompilation(GLuint shader);

        GLuint id = 0;
};