#include "Scene.h"

glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.01f, 100.0f);
glm::mat4 View = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
glm::mat4 ModelMat = glm::mat4(1.0f);

Scene::Scene()
{
    float square[] =
    {
        -0.85f,  0.25f, 0.0f,   
        -0.35f,  0.25f, 0.0f,   
        -0.85f, -0.25f, 0.0f,

        -0.35f,  0.25f, 0.0f,   
        -0.85f, -0.25f, 0.0f,   
        -0.35f, -0.25f, 0.0f
    };

    float triangle[] =
    {
         0.25f, 0.20f, 0.0f,
        -0.15f, -0.70f, 0.0f,
         0.65f, -0.70f, 0.0f
    };

    model = new Model();
    model->setupModel(square, sizeof(square));
    shaderProgram = new ShaderProgram("vertex_shader.glsl", "square_fragment_shader.glsl");
    object = new DrawableObject(model, shaderProgram, 6, GL_TRIANGLES);

    model2 = new Model();
    model2->setupModel(triangle, sizeof(triangle));
	shaderProgram2 = new ShaderProgram("vertex_shader.glsl", "triangle_fragment_shader.glsl");
    object2 = new DrawableObject(model2, shaderProgram2, 3, GL_TRIANGLES);
}

Scene::~Scene()
{
    delete object;
    delete shaderProgram;
    delete model;

	delete object2;
	delete shaderProgram2;
	delete model2;
}

void Scene::draw() 
{
    object->draw();
	object2->draw();
}