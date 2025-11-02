#pragma once
#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>
#include <cstdlib>

#include "Shader.h"
#include "Observer.h"

using namespace glm;

class ShaderProgram : public Observer
{
    public:
        ShaderProgram(const char* vertexSrc, const char* fragmentSrc);
        ~ShaderProgram();
        void use() const;
		void set(const char* name, int value) const;
		void set(const char* name, float value) const;
        void set(const char* name, const vec3& vec) const;
        void set(const char* name, const vec4& vec) const;
		void set(const char* name, const mat4& mat) const;
        void updateViewProjection(const mat4& view, const mat4& projection) override;

    private:
        void checkLinking(GLuint program);
        GLint uniformLoc(const char* name) const;

        GLuint programID = 0;
};
#endif