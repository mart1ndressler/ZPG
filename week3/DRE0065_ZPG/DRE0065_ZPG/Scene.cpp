#include "Scene.h"
#include "sphere.h"
#include "tree.h"
#include "bushes.h"

Scene::Scene()
{
	setupScene1();
    setupScene2();
	setupScene3();
}

Scene::~Scene()
{
	delete triangleObject;
	delete triangleShaderProgram;
	delete triangleModel;

    delete sphereObject;
    delete sphereShaderProgram;
    delete sphereModel;

	delete treeObject;
	delete treeShaderProgram;
	delete treeModel;

	delete bushObject;
	delete bushShaderProgram;
	delete bushModel;
}

void Scene::setScene(int id)
{
    if(currentScene == id) return;
    currentScene = id;
	fprintf(stderr, "Switching to scene %d\n", currentScene);
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
}

void Scene::setupScene3()
{
    treeModel = new Model();
    treeModel->setupModel(tree, sizeof(tree), true);
    treeShaderProgram = new ShaderProgram("vertex_shader.glsl", "tree_fragment_shader.glsl");
    treeObject = new DrawableObject(treeModel, treeShaderProgram, 92814, GL_TRIANGLES);

    treeInstances =
    {
        {-0.75f, -0.55f, radians(-20.0f), 0.08f}, {-0.25f, -0.55f, radians(25.0f), 0.10f},
        {0.25f, -0.55f, radians(-20.0f), 0.08f}, {0.75f, -0.55f, radians(25.0f), 0.10f},
        {-0.60f, 0.15f, radians(25.0f), 0.10f}, {-0.10f,  0.15f, radians(-20.0f), 0.08f},
        {0.40f, 0.15f, radians(25.0f), 0.10f}, {0.80f, 0.15f, radians(-20.0f), 0.08f},
    };

    bushModel = new Model();
    bushModel->setupModel(bushes, sizeof(bushes), true);
    bushShaderProgram = new ShaderProgram("vertex_shader.glsl", "bush_fragment_shader.glsl");
    bushObject = new DrawableObject(bushModel, bushShaderProgram, 8730, GL_TRIANGLES);

    bushInstances =
    {
        {-0.85f, -0.15f, radians(-10.0f), 0.22f}, {-0.45f, -0.15f, radians(15.0f), 0.24f},
        {-0.05f, -0.15f, radians(8.0f), 0.20f}, {0.35f, -0.15f, radians(-12.0f), 0.26f},
        {0.75f, -0.15f, radians(5.0f), 0.23f}, {-0.85f, 0.55f, radians(12.0f), 0.25f},
        {-0.45f, 0.55f, radians(-15.0f), 0.22f}, {-0.05f, 0.55f, radians(10.0f), 0.24f},
        {0.35f, 0.55f, radians(-8.0f), 0.21f}, {0.75f, 0.55f, radians(14.0f), 0.25f},
        {-0.25f, 0.35f, radians(-6.0f), 0.23f}, {0.55f, -0.35f, radians(9.0f), 0.22f},
    };

}

void Scene::draw()
{
    if(currentScene == 1)
    {
        triangleShaderProgram->use();
        vec2 center(0.25f, -0.40f);
        mat4 M(1.0f);

        TransformationComposite TC_triangle;
        TC_triangle.addComponent(new Rotation(0.5f * (float)glfwGetTime(), vec3(0.0f, 0.0f, 1.0f)));
        TC_triangle.addComponent(new Translation(vec3(-center.x, -center.y, 0.0f)));
        TC_triangle.apply(M);

		triangleObject->setModelMatrix(M);
        triangleObject->draw();
    }
    else if(currentScene == 2)
    {
        sphereShaderProgram->use();
        for(int i = 0; i < 4; ++i)
        {
            vec3 pos(0.0f);
            mat4 M(1.0f);

            if(i == 0) pos = vec3(0.40f, 0.0f, 0.0f);
            else if(i == 1) pos = vec3(-0.40f, 0.0f, 0.0f);
            else if(i == 2) pos = vec3(0.0f, 0.40f, 0.0f);
            else pos = vec3(0.0f, -0.40f, 0.0f);

            TransformationComposite TC_sphere;
            TC_sphere.addComponent(new Translation(pos));
            TC_sphere.addComponent(new Rotation(0.5f * (float)glfwGetTime(), vec3(0.0f, 1.0f, 0.0f)));
            TC_sphere.addComponent(new Scale(vec3(0.20f)));
            TC_sphere.apply(M);

			sphereObject->setModelMatrix(M);
            sphereObject->draw();
        }
    }
    else if(currentScene == 3)
    {
        mat4 projection = perspective(radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        mat4 view = lookAt(vec3(0.0f, 3.0f, 6.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
        mat4 VP = projection * view;

        treeShaderProgram->use();
        for(auto& trees : treeInstances)
        {
            mat4 M(1.0f);
            TransformationComposite TC_tree;
            TC_tree.addComponent(new Translation(vec3(trees.x * 3.0f, 0.0f, -trees.y * 3.0f)));
            TC_tree.addComponent(new Rotation(trees.z, vec3(0, 1, 0)));
            TC_tree.addComponent(new Scale(vec3(trees.w)));
            TC_tree.apply(M);

            treeObject->setModelMatrix(VP * M);
            treeObject->draw();
        }

        bushShaderProgram->use();
        for(auto& bushes : bushInstances)
        {
            mat4 M(1.0f);
            TransformationComposite TC_bush;
            TC_bush.addComponent(new Translation(vec3(bushes.x * 3.0f, 0.0f, -bushes.y * 3.0f)));
            TC_bush.addComponent(new Rotation(bushes.z, vec3(0, 1, 0)));
            TC_bush.addComponent(new Scale(vec3(bushes.w)));
			TC_bush.apply(M);

			bushObject->setModelMatrix(VP * M);
			bushObject->draw();
        }
    }
}