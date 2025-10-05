#pragma once
#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>
#include <cstdlib>

#include "Shader.h"

using namespace glm;

class ShaderProgram
{
    public:
        ShaderProgram(const char* vertexSrc, const char* fragmentSrc);
        ~ShaderProgram();
        void use() const;

		void set(const char* name, int value) const;
		void set(const char* name, float value) const;
        void set(const char* name, const vec3& vec) const;
		void set(const char* name, const mat4& mat) const;

    private:
        GLuint programID = 0;
        void checkLinking(GLuint program);
        GLint uniformLoc(const char* name) const;
};
#endif