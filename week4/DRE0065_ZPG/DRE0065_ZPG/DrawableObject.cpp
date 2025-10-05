#include "DrawableObject.h"

DrawableObject::DrawableObject(Model* m, ShaderProgram* s, int count, GLenum drawMode) : model(m), shaderprogram(s), vertexCount(count), mode(drawMode) {}

void DrawableObject::draw()
{
    shaderprogram->use();
    shaderprogram->set("modelMatrix", modelMatrix);
    glBindVertexArray(model->vao());
    glDrawArrays(mode, 0, vertexCount);
    glBindVertexArray(0);
}