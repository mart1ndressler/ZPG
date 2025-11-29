#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include "ShaderProgram.h"
#include "Material.h"

using namespace std;
using namespace glm;

class Model
{
    public:
        Model();
        Model(const char* name);
        ~Model();
        void setupModel(const float* data, int sizeInBytes, bool hasNormals, bool hasUV = false);
        void applyMdlMtl(ShaderProgram* shaderProgram);
        GLuint vao() const;
        int count() const;
        
        bool hasMtl;
        vec3 mtlDiffuse;
        float mtlShininess;

    private:
        GLuint VAO = 0, VBO = 0;
		int vertexCount = 0;
};