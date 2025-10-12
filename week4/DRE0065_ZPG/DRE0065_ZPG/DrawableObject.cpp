#include "DrawableObject.h"

DrawableObject::DrawableObject(Model* m, ShaderProgram* s, int count, GLenum drawMode) : model(m), shaderProgram(s), vertexCount(count), mode(drawMode) {}

void DrawableObject::render(Camera* camera, const mat4& modelMatrix)
{
    shaderProgram->use();
    shaderProgram->set("viewMatrix", camera->getViewMatrix());
    shaderProgram->set("projectionMatrix", camera->getProjectionMatrix());
    shaderProgram->set("modelMatrix", modelMatrix);
}

void DrawableObject::draw()
{
    glBindVertexArray(model->vao());
    glDrawArrays(mode, 0, vertexCount);
    glBindVertexArray(0);
}