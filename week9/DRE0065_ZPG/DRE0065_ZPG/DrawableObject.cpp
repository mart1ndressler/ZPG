#include "DrawableObject.h"

int DrawableObject::nextID = 1;

DrawableObject::DrawableObject(Model* model, ShaderProgram* shaderProgram, int count, GLenum drawMode) : model(model), shaderProgram(shaderProgram), vertexCount(count), mode(drawMode) 
{
    id = nextID++;
}

void DrawableObject::draw(const mat4& modelMatrix)
{
    shaderProgram->use();
    shaderProgram->set("modelMatrix", modelMatrix);

    GLuint stencilID = (GLuint)(id & 0xFF);
    if(stencilID == 0) stencilID = 1;
    glStencilFunc(GL_ALWAYS, stencilID, 0xFF);

    glBindVertexArray(model->vao());
    glDrawArrays(mode, 0, vertexCount);
    glBindVertexArray(0);
}

int DrawableObject::getID() const
{
    return id;
}