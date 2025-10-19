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
        DrawableObject(Model* model, ShaderProgram* shaderProgram, int vertexCount, GLenum drawMode);
        void draw(const mat4& modelMatrix);

    private:
        Model* model;
        ShaderProgram* shaderProgram;
        int vertexCount;
        GLenum mode;
};
#endif