#pragma once
#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp> 

#include "Model.h"
#include "ShaderProgram.h"
#include "TransformationComposite.h"

using namespace glm;

class DrawableObject
{
    public:
        DrawableObject(Model* model, ShaderProgram* shaderprogram, int vertexCount, GLenum drawMode);
        void setModelMatrix(const mat4& m) {modelMatrix = m;}
        void draw();

    private:
        Model* model;
        ShaderProgram* shaderprogram;
        int vertexCount;
        GLenum mode;
        mat4 modelMatrix{1.0f};
};
#endif