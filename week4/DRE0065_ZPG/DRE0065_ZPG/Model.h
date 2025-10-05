#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>

class Model
{
    public:
        Model();
        ~Model();
        void setupModel(const float* data, int sizeInBytes, bool hasNormals);
        GLuint vao() const {return VAO;}

    private:
        GLuint VAO = 0;
        GLuint VBO = 0;
};
#endif