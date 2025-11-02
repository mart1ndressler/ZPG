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

	lighting = new LightingController();
	environment = new EnvironmentController();

    setupScene1();
    setupScene2();
    setupScene3();
    setupScene4();
}

Scene::~Scene()
{
    for(auto* model : models) delete model;
    for(auto* shaderProgram : shaderPrograms) delete shaderProgram;
    for(auto* object : objects) delete object;

    delete camera1;
    delete camera2;
    delete pointLight;
	delete lighting;
	delete environment;
}

void Scene::setScene(int id)
{
    int prevScene = currentScene;
    currentScene = id;

    fprintf(stderr, "Switching to scene %d\n", currentScene);
    if(currentScene == 2)
    {
        scene2Mode = 4;
        scene2Lights = 1;

        activeCamera = camera1;
        camera1->reset(vec3(0.0f, 0.0f, 1.5f), -90.0f, 0.0f);
    }
    else if(currentScene == 3 || currentScene == 4)
    {
        activeCamera = camera2;
        camera2->reset(vec3(0.0f, 3.0f, 7.0f), -90.0f, -20.0f);

		forestLightMode = 0;
		flashlightOn = false;
    }

    if(camera1) camera1->setFov(60.0f);
    if(camera2) camera2->setFov(60.0f);
}

int Scene::getScene()
{
    return currentScene;
}

void Scene::updateCamera(GLFWwindow* window, float deltaTime, double xpos, double ypos, bool rightPressed)
{
    if(currentScene == 2 || currentScene == 3 || currentScene == 4)
    {
        activeCamera->processKeyboard(window, deltaTime);
        activeCamera->processMouse(xpos, ypos, rightPressed);
    }
}

void Scene::setAspect(float a)
{
    if(camera1) camera1->setAspect(a);
    if(camera2) camera2->setAspect(a);
}

void Scene::setFov(float f)
{
    if(activeCamera) activeCamera->setFov(f);
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
    triangleShaderProgram = new ShaderProgram("triangle.vert", "triangle.frag");
    triangleObject = new DrawableObject(triangleModel, triangleShaderProgram, 3, GL_TRIANGLES);

	models.push_back(triangleModel); shaderPrograms.push_back(triangleShaderProgram); objects.push_back(triangleObject);
}

void Scene::setupScene2()
{
    sphereModel = new Model();
    sphereModel->setupModel(sphere, sizeof(sphere), true);
    constantShaderProgram = new ShaderProgram("standard_lit.vert", "constant.frag");
    lambertShaderProgram = new ShaderProgram("standard_lit.vert", "lambert.frag");
    phongShaderProgram = new ShaderProgram("standard_lit.vert", "phong.frag");
    blinnShaderProgram = new ShaderProgram("standard_lit.vert", "blinn.frag");
    constantSphereObject = new DrawableObject(sphereModel, constantShaderProgram, 2880, GL_TRIANGLES);
    lambertSphereObject = new DrawableObject(sphereModel, lambertShaderProgram, 2880, GL_TRIANGLES);
    phongSphereObject = new DrawableObject(sphereModel, phongShaderProgram, 2880, GL_TRIANGLES);
    blinnSphereObject = new DrawableObject(sphereModel, blinnShaderProgram, 2880, GL_TRIANGLES);

    models.push_back(sphereModel);
    shaderPrograms.push_back(constantShaderProgram); shaderPrograms.push_back(lambertShaderProgram); shaderPrograms.push_back(phongShaderProgram); shaderPrograms.push_back(blinnShaderProgram);
    objects.push_back(constantSphereObject); objects.push_back(lambertSphereObject); objects.push_back(phongSphereObject); objects.push_back(blinnSphereObject);

    camera1->addObserver(constantShaderProgram);
    camera1->addObserver(lambertShaderProgram);
    camera1->addObserver(phongShaderProgram);
    camera1->addObserver(blinnShaderProgram);

    if(lambertShaderProgram) Material(1.0f, 1.0f, 0.0f, 16.0f).applyM(lambertShaderProgram);
    if(phongShaderProgram) Material(1.0f, 1.0f, 1.0f, 16.0f).applyM(phongShaderProgram);
    if(blinnShaderProgram) Material(1.0f, 1.0f, 1.0f, 128.0f).applyM(blinnShaderProgram);

    if(!pointLight) pointLight = new Light(vec3(0.0f, 0.0f, 0.0f));
    if(lighting) lighting->setPointLight(pointLight);
}

void Scene::setupScene3()
{
    plainModel = new Model();
    plainModel->setupModel(plain, sizeof(plain), true);
    plainShaderProgram = new ShaderProgram("standard_lit.vert", "forest.frag");
    plainObject = new DrawableObject(plainModel, plainShaderProgram, 6, GL_TRIANGLES);

    treeModel = new Model();
    treeModel->setupModel(tree, sizeof(tree), true);
    treeShaderProgram = new ShaderProgram("standard_lit.vert", "forest.frag");
    treeObject = new DrawableObject(treeModel, treeShaderProgram, 92814, GL_TRIANGLES);

    bushModel = new Model();
    bushModel->setupModel(bushes, sizeof(bushes), true);
    bushShaderProgram = new ShaderProgram("standard_lit.vert", "forest.frag");
    bushObject = new DrawableObject(bushModel, bushShaderProgram, 8730, GL_TRIANGLES);

    houseModel = new Model("house.obj");
    formulaModel = new Model("formula1.obj");
    modelsShaderProgram = new ShaderProgram("standard_lit.vert", "forest.frag");
    houseObject = new DrawableObject(houseModel, modelsShaderProgram, houseModel->count(), GL_TRIANGLES);
    formulaObject = new DrawableObject(formulaModel, modelsShaderProgram, formulaModel->count(), GL_TRIANGLES);

    fireflyShaderProgram = new ShaderProgram("standard_lit.vert", "firefly.frag");
    fireflyObject = new DrawableObject(sphereModel, fireflyShaderProgram, 2880, GL_TRIANGLES);

	models.push_back(plainModel); models.push_back(treeModel); models.push_back(bushModel); models.push_back(houseModel); models.push_back(formulaModel), 
	shaderPrograms.push_back(plainShaderProgram); shaderPrograms.push_back(treeShaderProgram); shaderPrograms.push_back(bushShaderProgram); shaderPrograms.push_back(modelsShaderProgram); shaderPrograms.push_back(fireflyShaderProgram);
	objects.push_back(plainObject); objects.push_back(treeObject); objects.push_back(bushObject); objects.push_back(houseObject); objects.push_back(formulaObject); objects.push_back(fireflyObject);

	camera2->addObserver(plainShaderProgram);
	camera2->addObserver(treeShaderProgram);
	camera2->addObserver(bushShaderProgram);
    camera2->addObserver(fireflyShaderProgram);
	camera2->addObserver(modelsShaderProgram);

    if(plainShaderProgram) Material(vec3(0.07f, 0.25f, 0.07f), 1.0f, 1.0f, 0.0f, 16.0f).applyM(plainShaderProgram);
    if(treeShaderProgram) Material(vec3(0.06f, 0.18f, 0.08f), 1.0f, 1.0f, 0.0f, 16.0f).applyM(treeShaderProgram);
    if(bushShaderProgram) Material(vec3(0.20f, 0.65f, 0.28f), 1.0f, 1.0f, 0.0f, 16.0f).applyM(bushShaderProgram);
    if(modelsShaderProgram) Material(vec3(0.70f, 0.70f, 0.70f), 1.0f, 1.0f, 0.0f, 16.0f).applyM(modelsShaderProgram);
    
    srand((unsigned int)time(NULL));
    treeInstances.clear(); bushInstances.clear();
    if(environment)
    {
        vector<vec4> avoidModels = {vec4(0.0f, 0.0f, 0.0f, 0.0f)};
        float houseAvoidTrees = 0.55f, houseAvoidBushes = 0.45f;

        environment->generateInstances(treeInstances, 200, 0.10f, houseAvoidTrees, 0.07f, 0.10f, &avoidModels);
        environment->generateInstances(bushInstances, 50, 0.03f, houseAvoidBushes, 0.18f, 0.22f, &avoidModels);
    }
}

void Scene::setupScene4()
{
    solarShaderProgram = new ShaderProgram("standard_lit.vert", "solar.frag");
    shaderPrograms.push_back(solarShaderProgram);

    sunObject = new DrawableObject(sphereModel, solarShaderProgram, 2880, GL_TRIANGLES);
    earthObject = new DrawableObject(sphereModel, solarShaderProgram, 2880, GL_TRIANGLES);
    moonObject = new DrawableObject(sphereModel, solarShaderProgram, 2880, GL_TRIANGLES);

    objects.push_back(sunObject); objects.push_back(earthObject); objects.push_back(moonObject);
    camera2->addObserver(solarShaderProgram);

    if(solarShaderProgram) Material(1.0f, 1.0f, 1.0f, 32.0f).applyM(solarShaderProgram);

    if(!pointLight) pointLight = new Light(vec3(0.0f, 0.0f, 0.0f));
    if(lighting) lighting->setPointLight(pointLight);
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

        triangleObject->draw(M);
    }
    else if(currentScene == 2)
    {
        if(lighting)
        {
            lighting->applyL(constantShaderProgram, currentScene, scene2Lights);
            lighting->applyL(lambertShaderProgram, currentScene, scene2Lights);
            lighting->applyL(phongShaderProgram, currentScene, scene2Lights);
            lighting->applyL(blinnShaderProgram, currentScene, scene2Lights);
        }

        DrawableObject* sphereObjects[4] = {constantSphereObject, lambertSphereObject, phongSphereObject, blinnSphereObject};
        DrawableObject* modeObjects[4];

        if(scene2Mode == 0) modeObjects[0] = modeObjects[1] = modeObjects[2] = modeObjects[3] = constantSphereObject;
        else if(scene2Mode == 1) modeObjects[0] = modeObjects[1] = modeObjects[2] = modeObjects[3] = phongSphereObject;
        else if(scene2Mode == 2) modeObjects[0] = modeObjects[1] = modeObjects[2] = modeObjects[3] = lambertSphereObject;
        else if(scene2Mode == 3) modeObjects[0] = modeObjects[1] = modeObjects[2] = modeObjects[3] = blinnSphereObject;
        else for(int i = 0; i < 4; i++) modeObjects[i] = sphereObjects[i];

        vec3 positions[4] = {vec3(0.40f, 0.0f, 0.0f), vec3(-0.40f, 0.0f, 0.0f), vec3(0.0f, 0.40f, 0.0f), vec3(0.0f, -0.40f, 0.0f)};
        for(int i = 0; i < 4; i++)
        {
            mat4 M(1.0f);
            TransformationComposite TC;
            TC.addComponent(new Translation(positions[i]));
            TC.addComponent(new Scale(vec3(0.20f)));
            TC.apply(M);

            modeObjects[i]->draw(M);
        }
    }
    else if(currentScene == 3)
    {
        if(environment) environment->updateFireflies();

        if(lighting && environment)
        {
            vec3 camPos = activeCamera->getPosition();
            vec3 camDir = activeCamera->getFront();

            lighting->applyL(plainShaderProgram, currentScene, 0, forestLightMode, environment->fireflyPositions, environment->fireflyTotal, camPos, camDir, flashlightOn);
            lighting->applyL(treeShaderProgram, currentScene, 0, forestLightMode, environment->fireflyPositions, environment->fireflyTotal, camPos, camDir, flashlightOn);
            lighting->applyL(bushShaderProgram, currentScene, 0, forestLightMode, environment->fireflyPositions, environment->fireflyTotal, camPos, camDir, flashlightOn);
            lighting->applyL(modelsShaderProgram, currentScene, 0, forestLightMode, environment->fireflyPositions, environment->fireflyTotal, camPos, camDir, flashlightOn);
        }

        mat4 M(1.0f);
        TransformationComposite TC_plain;
        TC_plain.addComponent(new Scale(vec3(3.2f, 1.0f, 3.2f)));
        TC_plain.apply(M);

        plainObject->draw(M);

        for(auto& trees : treeInstances)
        {
            mat4 M(1.0f);
            TransformationComposite TC_tree;
            TC_tree.addComponent(new Translation(vec3(trees.x * 3.0f, 0.0f, -trees.y * 3.0f)));
            TC_tree.addComponent(new Rotation(trees.z, vec3(0, 1, 0)));
            TC_tree.addComponent(new Scale(vec3(trees.w)));
            TC_tree.apply(M);

            treeObject->draw(M);
        }

        for(auto& bushes : bushInstances)
        {
            mat4 M(1.0f);
            TransformationComposite TC_bush;
            TC_bush.addComponent(new Translation(vec3(bushes.x * 3.0f, 0.0f, -bushes.y * 3.0f)));
            TC_bush.addComponent(new Rotation(bushes.z, vec3(0, 1, 0)));
            TC_bush.addComponent(new Scale(vec3(bushes.w)));
            TC_bush.apply(M);

            bushObject->draw(M);
        }

        if(environment && lighting && (forestLightMode == 1 || forestLightMode == 3))
        {
            int n = lighting->fireflyCap;
            if(n > environment->fireflyTotal) n = environment->fireflyTotal;
            if(n > 16) n = 16;

            for(int i = 0; i < n; i++)
            {
                mat4 M(1.0f);
                TransformationComposite TC_firefly;
                TC_firefly.addComponent(new Translation(environment->fireflyPositions[i]));
                TC_firefly.addComponent(new Scale(vec3(0.04f)));
                TC_firefly.apply(M);

                fireflyObject->draw(M);
            }
        }

        mat4 M_house(1.0f);
        TransformationComposite TC_house;
        TC_house.addComponent(new Translation(vec3(0.0f, 0.0f, 0.0f)));
        TC_house.addComponent(new Scale(vec3(0.10f)));
        TC_house.apply(M_house);

        houseObject->draw(M_house);

        mat4 M_formula(1.0f);
        TransformationComposite TC_formula;
        TC_formula.addComponent(new Translation(vec3(0.90f, 0.0f, 0.0f)));
        TC_formula.addComponent(new Rotation(radians(90.0f), vec3(0, 1, 0)));
        TC_formula.addComponent(new Scale(vec3(0.02f)));
        TC_formula.apply(M_formula);

        formulaObject->draw(M_formula);
    }
    else if(currentScene == 4)
    {
        if(pointLight) pointLight->setPosition(vec3(0.0f));
        if(lighting) lighting->applyL(solarShaderProgram, 4, 0);

        vec3 earthPos = vec3(2.0f * cos(0.50f * (float)glfwGetTime()), 2.0f * sin(0.50f * (float)glfwGetTime()), 0.0f);
        vec3 moonPos = vec3(0.60f * cos(1.50f * (float)glfwGetTime()), 0.60f * sin(1.50f * (float)glfwGetTime()), 0.0f);

        vec3 colors[3] = {vec3(1.0f, 0.93f, 0.30f), vec3(0.20f, 0.45f, 0.90f), vec3(0.65f, 0.65f, 0.65f)};
        vec3 scales[3] = {vec3(1.0f), vec3(0.35f), vec3(0.12f)};
        vec3 positions[3] = {vec3(0.0f), earthPos, earthPos + moonPos};
        float emissive[3] = {0.8f, 0.0f, 0.0f};

        DrawableObject* objects[3] = {sunObject, earthObject, moonObject};
        for(int i = 0; i < 3; i++)
        {
            mat4 M(1.0f);
            TransformationComposite TC;
            TC.addComponent(new Translation(positions[i]));
            TC.addComponent(new Scale(scales[i]));
            TC.apply(M);

            solarShaderProgram->use();
            solarShaderProgram->set("objectColor", colors[i]);
            solarShaderProgram->set("emissive", emissive[i]);

            objects[i]->draw(M);
        }
    }
}