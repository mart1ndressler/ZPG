#include "Scene.h"
#include "sphere.h"
#include "tree.h"
#include "bushes.h"
#include "plain.h"

Scene::Scene()
{
    camera1 = new Camera(vec3(0.0f, 0.0f, 1.5f), -90.0f, 0.0f, 800.0f / 600.0f);
    camera2 = new Camera(vec3(0.0f, 3.0f, 7.0f), -90.0f, -20.0f, 800.0f / 600.0f);
    activeCamera = camera1;

    setupScene1();
    setupScene2();
    setupScene3();
}

Scene::~Scene()
{
    Model* models[] = {triangleModel, sphereModel, plainModel, treeModel, bushModel};
    ShaderProgram* shaders[] = {triangleShaderProgram, sphereShaderProgram, plainShaderProgram, treeShaderProgram, bushShaderProgram};
    DrawableObject* objects[] = {triangleObject, sphereObject, plainObject, treeObject, bushObject};

    for(int i = 0; i < 5; i++)
    {
        delete objects[i];
        delete shaders[i];
        delete models[i];
    }
}

void Scene::setScene(int id)
{
    bool isSameScene = (currentScene == id);
    currentScene = id;

    if(!isSameScene) fprintf(stderr, "Switching to scene %d\n", currentScene);
    if(currentScene == 2)
    {
        activeCamera = camera1;
        camera1->reset(vec3(0.0f, 0.0f, 1.5f), -90.0f, 0.0f);
    }
    else if(currentScene == 3)
    {
        activeCamera = camera2;
        camera2->reset(vec3(0.0f, 3.0f, 7.0f), -90.0f, -20.0f);
    }
}

void Scene::updateCamera(GLFWwindow* window, float deltaTime, double xpos, double ypos, bool rightPressed)
{
    if(currentScene == 2 || currentScene == 3)
    {
        activeCamera->processKeyboard(window, deltaTime);
        activeCamera->processMouse(xpos, ypos, rightPressed);
    }
}

void Scene::setupScene1()
{
    float triangle[] =
    {
        0.25f,  0.20f, 0.0f,
       -0.15f, -0.70f, 0.0f,
        0.65f, -0.70f, 0.0f
    };

    triangleModel = new Model();
    triangleModel->setupModel(triangle, sizeof(triangle), false);
    triangleShaderProgram = new ShaderProgram("vertex_shader.glsl", "triangle_fragment_shader.glsl");
    triangleObject = new DrawableObject(triangleModel, triangleShaderProgram, 3, GL_TRIANGLES);
}

void Scene::setupScene2()
{
    sphereModel = new Model();
    sphereModel->setupModel(sphere, sizeof(sphere), true);
    sphereShaderProgram = new ShaderProgram("vertex_shader.glsl", "triangle_fragment_shader.glsl");
    sphereObject = new DrawableObject(sphereModel, sphereShaderProgram, 2880, GL_TRIANGLES);

	camera1->addObserver(sphereShaderProgram);
}

void Scene::setupScene3()
{
    plainModel = new Model();
    plainModel->setupModel(plain, sizeof(plain), true);
    plainShaderProgram = new ShaderProgram("vertex_shader.glsl", "plain_fragment_shader.glsl");
    plainObject = new DrawableObject(plainModel, plainShaderProgram, 6, GL_TRIANGLES);

    treeModel = new Model();
    treeModel->setupModel(tree, sizeof(tree), true);
    treeShaderProgram = new ShaderProgram("vertex_shader.glsl", "tree_fragment_shader.glsl");
    treeObject = new DrawableObject(treeModel, treeShaderProgram, 92814, GL_TRIANGLES);

    bushModel = new Model();
    bushModel->setupModel(bushes, sizeof(bushes), true);
    bushShaderProgram = new ShaderProgram("vertex_shader.glsl", "bush_fragment_shader.glsl");
    bushObject = new DrawableObject(bushModel, bushShaderProgram, 8730, GL_TRIANGLES);

	camera2->addObserver(plainShaderProgram);
	camera2->addObserver(treeShaderProgram);
	camera2->addObserver(bushShaderProgram);

    srand(time(NULL));
    for(int i = 0; i < 50; i++) treeInstances.push_back(vec4(((rand() % 200) / 100.0f) - 1.0f, ((rand() % 200) / 100.0f) - 1.0f, radians((float)(rand() % 360)), 0.07f + ((float)(rand() % 3) * 0.01f)));
    for(int i = 0; i < 50; i++) bushInstances.push_back(vec4(((rand() % 200) / 100.0f) - 1.0f, ((rand() % 200) / 100.0f) - 1.0f, radians((float)(rand() % 360)), 0.18f + ((float)(rand() % 3) * 0.02f)));
}

void Scene::draw()
{
    if(currentScene == 1)
    {
        mat4 M(1.0f);
        TransformationComposite TC_triangle;
        TC_triangle.addComponent(new Rotation(0.5f * (float)glfwGetTime(), vec3(0.0f, 0.0f, 1.0f)));
        TC_triangle.addComponent(new Translation(vec3(-0.25f, 0.40f, 0.0f)));
        TC_triangle.apply(M);

        triangleObject->render(activeCamera, M);
        triangleObject->draw();
    }
    else if(currentScene == 2)
    {
        vec3 positions[4] = {vec3(0.40f, 0.0f, 0.0f), vec3(-0.40f, 0.0f, 0.0f), vec3(0.0f, 0.40f, 0.0f), vec3(0.0f, -0.40f, 0.0f)};
        for(int i = 0; i < 4; i++)
        {
            mat4 M(1.0f);
            TransformationComposite TC_sphere;
            TC_sphere.addComponent(new Translation(positions[i]));
            TC_sphere.addComponent(new Rotation(0.5f * (float)glfwGetTime(), vec3(0.0f, 1.0f, 0.0f)));
            TC_sphere.addComponent(new Scale(vec3(0.20f)));
            TC_sphere.apply(M);

            sphereObject->render(activeCamera, M);
            sphereObject->draw();
        }
    }
    else if(currentScene == 3)
    {
        mat4 M(1.0f);
        TransformationComposite TC_plain;
        TC_plain.addComponent(new Scale(vec3(3.2f, 1.0f, 3.2f)));
        TC_plain.apply(M);

        plainObject->render(activeCamera, M);
        plainObject->draw();

        for(auto& trees : treeInstances)
        {
            mat4 M(1.0f);
            TransformationComposite TC_tree;
            TC_tree.addComponent(new Translation(vec3(trees.x * 3.0f, 0.0f, -trees.y * 3.0f)));
            TC_tree.addComponent(new Rotation(trees.z, vec3(0, 1, 0)));
            TC_tree.addComponent(new Scale(vec3(trees.w)));
            TC_tree.apply(M);

            treeObject->render(activeCamera, M);
            treeObject->draw();
        }

        for(auto& bushes : bushInstances)
        {
            mat4 M(1.0f);
            TransformationComposite TC_bush;
            TC_bush.addComponent(new Translation(vec3(bushes.x * 3.0f, 0.0f, -bushes.y * 3.0f)));
            TC_bush.addComponent(new Rotation(bushes.z, vec3(0, 1, 0)));
            TC_bush.addComponent(new Scale(vec3(bushes.w)));
            TC_bush.apply(M);

            bushObject->render(activeCamera, M);
            bushObject->draw();
        }
    }
}