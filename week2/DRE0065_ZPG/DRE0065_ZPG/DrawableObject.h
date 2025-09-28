#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include <GL/glew.h>

#include "Model.h"
#include "ShaderProgram.h"

class DrawableObject
{
    public:
        DrawableObject(Model* model, ShaderProgram* shader, int vertexCount, GLenum drawMode);
        void draw();

    private:
        Model* model;
        ShaderProgram* shader;
        int vertexCount;
        GLenum mode;
};
#endif