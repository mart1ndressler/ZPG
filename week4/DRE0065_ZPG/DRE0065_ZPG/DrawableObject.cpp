#include "DrawableObject.h"

DrawableObject::DrawableObject(Model* m, ShaderProgram* s, int count, GLenum drawMode) : model(m), shaderprogram(s), vertexCount(count), mode(drawMode) {}

void DrawableObject::render(Camera* camera, const mat4& modelMatrix)
{
    shaderprogram->use();
    shaderprogram->set("viewMatrix", camera->getViewMatrix());
    shaderprogram->set("projectionMatrix", camera->getProjectionMatrix());
    shaderprogram->set("modelMatrix", modelMatrix);
}

void DrawableObject::draw()
{
    glBindVertexArray(model->vao());
    glDrawArrays(mode, 0, vertexCount);
    glBindVertexArray(0);
}