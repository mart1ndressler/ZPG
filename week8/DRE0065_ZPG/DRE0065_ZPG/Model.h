#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

using namespace std;

class Model
{
    public:
        Model();
        Model(const char* name);
        ~Model();
        void setupModel(const float* data, int sizeInBytes, bool hasNormals, bool hasUV = false);
        GLuint vao() const;
        int count() const;

    private:
        GLuint VAO = 0, VBO = 0;
		int vertexCount = 0;
};