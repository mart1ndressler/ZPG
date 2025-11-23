#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp> 
#include "Model.h"
#include "ShaderProgram.h"

using namespace glm;

class DrawableObject
{
    public:
        DrawableObject(Model* model, ShaderProgram* shaderProgram, int vertexCount, GLenum drawMode);
        void draw(const mat4& modelMatrix);
        int getID() const;

    private:
        Model* model;
        ShaderProgram* shaderProgram;
        int vertexCount, id;
		static int nextID;
        GLenum mode;
};