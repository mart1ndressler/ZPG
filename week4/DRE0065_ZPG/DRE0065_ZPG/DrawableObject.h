#pragma once
#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp> 

#include "Model.h"
#include "ShaderProgram.h"
#include "TransformationComposite.h"
#include "Camera.h"

using namespace glm;

class DrawableObject
{
    public:
        DrawableObject(Model* model, ShaderProgram* shaderProgram, int vertexCount, GLenum drawMode);

        void render(Camera* camera, const mat4& modelMatrix);
        void draw();

    private:
        Model* model;
        ShaderProgram* shaderProgram;
        int vertexCount;
        GLenum mode;
};
#endif