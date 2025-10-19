#include "DrawableObject.h"

DrawableObject::DrawableObject(Model* m, ShaderProgram* s, int count, GLenum drawMode) : model(m), shaderProgram(s), vertexCount(count), mode(drawMode) {}

void DrawableObject::draw(const mat4& modelMatrix)
{
    shaderProgram->use();
    shaderProgram->set("modelMatrix", modelMatrix);

    glBindVertexArray(model->vao());
    glDrawArrays(mode, 0, vertexCount);
    glBindVertexArray(0);
}