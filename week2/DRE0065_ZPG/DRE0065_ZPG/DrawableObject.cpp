#include "DrawableObject.h"

DrawableObject::DrawableObject(Model* m, ShaderProgram* s, int count, GLenum drawMode) : model(m), shader(s), vertexCount(count), mode(drawMode) {}

void DrawableObject::draw()
{
    shader->use();
    glBindVertexArray(model->vao());
    glDrawArrays(mode, 0, vertexCount);
    glBindVertexArray(0);
}