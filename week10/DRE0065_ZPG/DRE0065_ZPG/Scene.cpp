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
	setupScene5();
    setTransformMode(transformMode);
}

Scene::~Scene()
{
    for(auto* model : models) delete model;
    for(auto* shaderProgram : shaderPrograms) delete shaderProgram;
    for(auto* object : objects) delete object;
	for(auto* texture : textures) delete texture;

    delete camera1;
    delete camera2;
    delete pointLight;
	delete lighting;
	delete environment;
    delete skybox;
	delete solarSkybox;
}

void Scene::setScene(int id)
{
    bool isNewScene = (id != currentScene);
    currentScene = id;

    if(isNewScene) fprintf(stderr, "Switching to scene %d\n", currentScene);
    if(currentScene == 2)
    {
        scene2Mode = 4;
        scene2Lights = 1;

        activeCamera = camera1;
        camera1->reset(vec3(0.0f, 0.0f, 1.5f), -90.0f, 0.0f);
    }
    else if (currentScene == 3 || currentScene == 4 || currentScene == 5)
    {
        activeCamera = camera2;
        camera2->reset(vec3(0.0f, 3.0f, 7.0f), -90.0f, -20.0f);

        forestLightMode = 0;
        flashlightOn = false;

        (currentScene == 3 || currentScene == 4) ? skyboxOn = true : skyboxOn = false;
    }
    transformMode = 0;
    setTransformMode(transformMode);
    if(camera1) camera1->setFov(60.0f);
}

int Scene::getScene()
{
    return currentScene;
}

void Scene::updateCamera(GLFWwindow* window, float deltaTime, double xpos, double ypos, bool rightPressed)
{
    if(currentScene == 2 || currentScene == 3 || currentScene == 4 || currentScene == 5)
    {
        //float speedFactor = (transformMode == 2) ? 1.0f / 20.0f : 1.0f;
        activeCamera->processKeyboard(window, deltaTime/* * speedFactor */);
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

void Scene::handleClick(int mouseX, int mouseY, int width, int height)
{
    GLint x = mouseX, y = mouseY, newY = height - y;
    GLbyte color[4];
    GLfloat depth;
    GLuint index;

    glReadPixels(x, newY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
    glReadPixels(x, newY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    glReadPixels(x, newY, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

    const char* objectName = "None";
    DrawableObject* picked = nullptr;
    if(index != 0)
    {
        for(auto* object : objects)
        {
            if(!object) continue;
            GLuint stencilID = (GLuint)(object->getID() & 0xFF);
            if(stencilID == 0) stencilID = 1;
            if(stencilID == index)
            {
                picked = object;
                break;
            }
        }

        if(picked)
        {
            if(picked == triangleObject) objectName = "Triangle (Sc.1)";
            else if(picked == constantSphereObject) objectName = "Sphere-Constant (Sc.2)";
            else if(picked == lambertSphereObject) objectName = "Sphere-Lambert (Sc.2)";
            else if(picked == phongSphereObject) objectName = "Sphere-Phong (Sc.2)";
            else if(picked == blinnSphereObject) objectName = "Sphere-Blinn (Sc.2)";
            else if(picked == plainObject) objectName = "Plain (Sc.3)";
            else if(picked == treeObject) objectName = "Tree (Sc.3)";
            else if(picked == bushObject) objectName = "Bush (Sc.3)";
            else if(picked == houseObject) objectName = "House (Sc.3)";
            else if(picked == formulaObject) objectName = "F1 (Sc.3)";
            else if(picked == cubeObject) objectName = "Cube (Sc.3)";
            else if(picked == shrekObject) objectName = "Shrek (Sc.3/Sc.5)";
            else if(picked == fionaObject) objectName = "Fiona (Sc.3)";
            else if(picked == toiletObject) objectName = "Toilet (Sc.3)";
            else if(picked == fireflyObject) objectName = "Firefly (Sc.3)";
            else if(picked == sunObject) objectName = "Sun (Sc.4)";
            else if(picked == mercuryObject) objectName = "Mercury (Sc.4)";
            else if(picked == venusObject) objectName = "Venus (Sc.4)";
            else if(picked == earthObject) objectName = "Earth (Sc.4)";
			else if(picked == marsObject) objectName = "Mars (Sc.4)";
            else if(picked == jupiterObject) objectName = "Jupiter (Sc.4)";
            else if(picked == saturnObject) objectName = "Saturn (Sc.4)";
            else if(picked == uranusObject) objectName = "Uranus (Sc.4)";
			else if(picked == neptuneObject) objectName = "Neptune (Sc.4)";
            else if(picked == moonObject) objectName = "Moon (Sc.4)";
            else if(picked == arcadePlainObject) objectName = "Arcade Plain (Sc.5)";
            else if(picked == arcadeloginObject) objectName = "Login (Sc.5)";
        }

        if(currentScene == 5 && picked == shrekObject)
        {
            vec3 screenPoint((float)x, (float)newY, depth);
            vec4 viewport(0.0f, 0.0f, (float)width, (float)height);
            vec3 worldPos = unProject(screenPoint, activeCamera->getViewMatrix(), activeCamera->getProjectionMatrix(), viewport);

            int hitIdx = -1;
            float bestDist2 = 1e6f;
            for(int i = 0; i < 3; i++)
            {
				int cell = arcadeShrekPosIndex[i];
                cell = (cell < 0 || cell >= 36) ? 0 : cell;

                vec3 p = arcadePositions[cell], d = p - worldPos;
                float dist2 = dot(d, d);
                if(dist2 < bestDist2)
                {
                    bestDist2 = dist2;
                    hitIdx = i;
                }
            }

            if(hitIdx >= 0)
            {
                arcadeScore += 3;
                arcadeShrekLastTime[hitIdx] = (float)glfwGetTime();

                int oldIndex = arcadeShrekPosIndex[hitIdx], newIndex = oldIndex;
                while(newIndex == oldIndex) newIndex = rand() % 36;
                arcadeShrekPosIndex[hitIdx] = newIndex;
                fprintf(stderr, "[ARCADE] Hit Shrek! Score = %d\n", arcadeScore);
            }
        }

        if(picked == plainObject)
        {
            if(transformMode == 2) fprintf(stderr, "[PLANT] Skipped planting in transformMode=2!\n");
            else
            {
                vec3 screenPoint((float)x, (float)newY, depth);
                vec4 viewport(0.0f, 0.0f, (float)width, (float)height);

                vec3 worldPos = unProject(screenPoint, activeCamera->getViewMatrix(), activeCamera->getProjectionMatrix(), viewport);
                fprintf(stderr, "[UNPROJECT] worldPos = [%f, %f, %f]\n", worldPos.x, worldPos.y, worldPos.z);
                environment->plantInstanceAt(worldPos, forestExtent, treeInstances);
            }
        }
    }
    if(currentScene != 5) fprintf(stderr, "[PICK] Clicked pixel(%d, %d -> newY=%d), ""color=%02hhx%02hhx%02hhx%02hhx, depth=%f, stencil index=%u, object=%s\n",x, y, newY, color[0], color[1], color[2], color[3], depth, index, objectName);
}

void Scene::setTransformMode(int mode)
{
    int oldMode = transformMode;
    transformMode = mode;
    customMatrix[3][3] = 20.0f;

    if(mode != oldMode) fprintf(stderr, "[TRANSFORM] mode = %d\n", transformMode);
    for(ShaderProgram* shaderProgram : shaderPrograms)
    {
        if(!shaderProgram) continue;
        shaderProgram->updateTransform(customMatrix, transformMode);
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
    if(pointLight)
    {
        if(constantShaderProgram) pointLight->addObserver(constantShaderProgram);
        if(lambertShaderProgram) pointLight->addObserver(lambertShaderProgram);
        if(phongShaderProgram) pointLight->addObserver(phongShaderProgram);
        if(blinnShaderProgram) pointLight->addObserver(blinnShaderProgram);
    }
}

void Scene::setupScene3()
{
    plainModel = new Model();
    plainModel->setupModel(plain, sizeof(plain), true, true);
    treeModel = new Model();
    treeModel->setupModel(tree, sizeof(tree), true);
    bushModel = new Model();
    bushModel->setupModel(bushes, sizeof(bushes), true);

    houseModel = new Model("house.obj");
    formulaModel = new Model("formula1.obj");
    cubeModel = new Model("cube.obj");
    shrekModel = new Model("shrek.obj");
    toiletModel = new Model("toiled.obj");
    fionaModel = new Model("fiona.obj");

    plainShaderProgram = new ShaderProgram("standard_lit.vert", "forest_arcade.frag");
    treeShaderProgram = new ShaderProgram("standard_lit.vert", "forest_arcade.frag");
    bushShaderProgram = new ShaderProgram("standard_lit.vert", "forest_arcade.frag");
    modelsShaderProgram = new ShaderProgram("standard_lit.vert", "forest_arcade.frag");
    fireflyShaderProgram = new ShaderProgram("standard_lit.vert", "firefly.frag");
	skyboxShaderProgram = new ShaderProgram("standard_lit.vert", "forest_arcade.frag");

    plainObject = new DrawableObject(plainModel, plainShaderProgram, 6, GL_TRIANGLES);
    treeObject = new DrawableObject(treeModel, treeShaderProgram, 92814, GL_TRIANGLES);
    bushObject = new DrawableObject(bushModel, bushShaderProgram, 8730, GL_TRIANGLES);
    houseObject = new DrawableObject(houseModel, modelsShaderProgram, houseModel->count(), GL_TRIANGLES);
    formulaObject = new DrawableObject(formulaModel, modelsShaderProgram, formulaModel->count(), GL_TRIANGLES);
    cubeObject = new DrawableObject(cubeModel, modelsShaderProgram, cubeModel->count(), GL_TRIANGLES);
    shrekObject = new DrawableObject(shrekModel, modelsShaderProgram, shrekModel->count(), GL_TRIANGLES);
    toiletObject = new DrawableObject(toiletModel, modelsShaderProgram, toiletModel->count(), GL_TRIANGLES);
    fionaObject = new DrawableObject(fionaModel, modelsShaderProgram, fionaModel->count(), GL_TRIANGLES);
    fireflyObject = new DrawableObject(sphereModel, fireflyShaderProgram, 2880, GL_TRIANGLES);

    grassTexture = new Texture("grass.jpg");
    houseTexture = new Texture("house.png");
    formulaTexture = new Texture("wood.png");
    cubeTexture = new Texture("wood.png");
    shrekTexture = new Texture("shrek.png");
    toiletTexture = new Texture("toiled.jpg");
    fionaTexture = new Texture("fiona.png");

    skybox = new Skybox("posx.jpg", "negx.jpg", "posy.jpg", "negy.jpg", "posz.jpg", "negz.jpg");

	models.push_back(plainModel); models.push_back(treeModel); models.push_back(bushModel); models.push_back(houseModel); models.push_back(formulaModel), models.push_back(cubeModel); models.push_back(shrekModel); models.push_back(toiletModel); models.push_back(fionaModel);
	shaderPrograms.push_back(plainShaderProgram); shaderPrograms.push_back(treeShaderProgram); shaderPrograms.push_back(bushShaderProgram); shaderPrograms.push_back(modelsShaderProgram); shaderPrograms.push_back(fireflyShaderProgram), shaderPrograms.push_back(skyboxShaderProgram);
	objects.push_back(plainObject); objects.push_back(treeObject); objects.push_back(bushObject); objects.push_back(houseObject), objects.push_back(formulaObject); objects.push_back(cubeObject); objects.push_back(shrekObject); objects.push_back(toiletObject); objects.push_back(fionaObject); objects.push_back(fireflyObject);
	textures.push_back(grassTexture); textures.push_back(houseTexture); textures.push_back(formulaTexture); textures.push_back(cubeTexture); textures.push_back(shrekTexture); textures.push_back(toiletTexture); textures.push_back(fionaTexture);

	camera2->addObserver(plainShaderProgram);
	camera2->addObserver(treeShaderProgram);
	camera2->addObserver(bushShaderProgram);
    camera2->addObserver(fireflyShaderProgram);
	camera2->addObserver(modelsShaderProgram);
	camera2->addObserver(skyboxShaderProgram);

    if(plainShaderProgram) Material(vec3(0.07f, 0.25f, 0.07f), 1.0f, 1.0f, 0.0f, 16.0f).applyM(plainShaderProgram);
    if(treeShaderProgram) Material(vec3(0.06f, 0.18f, 0.08f), 1.0f, 1.0f, 0.0f, 16.0f).applyM(treeShaderProgram);
    if(bushShaderProgram) Material(vec3(0.20f, 0.65f, 0.28f), 1.0f, 1.0f, 0.0f, 16.0f).applyM(bushShaderProgram);
    
    srand((unsigned int)time(NULL));
    treeInstances.clear(); bushInstances.clear();
    if(environment)
    {
        vector<vec4> avoidModels = {vec4(0.0f, 0.0f, 0.0f, 0.0f)};
        float houseAvoid_T_B = 0.55f;

        environment->generateInstances(treeInstances, 500, 0.10f, houseAvoid_T_B, 0.07f, 0.10f, &avoidModels);
        environment->generateInstances(bushInstances, 250, 0.03f, houseAvoid_T_B, 0.18f, 0.22f, &avoidModels);
    }
}

void Scene::setupScene4()
{
	planetModel = new Model("planet.obj");
    solarShaderProgram = new ShaderProgram("standard_lit.vert", "solar.frag");

    sunObject = new DrawableObject(planetModel, solarShaderProgram, planetModel->count(), GL_TRIANGLES);
    earthObject = new DrawableObject(planetModel, solarShaderProgram, planetModel->count(), GL_TRIANGLES);
    moonObject = new DrawableObject(planetModel, solarShaderProgram, planetModel->count(), GL_TRIANGLES);
    mercuryObject = new DrawableObject(planetModel, solarShaderProgram, planetModel->count(), GL_TRIANGLES);
    venusObject = new DrawableObject(planetModel, solarShaderProgram, planetModel->count(), GL_TRIANGLES);
    marsObject = new DrawableObject(planetModel, solarShaderProgram, planetModel->count(), GL_TRIANGLES);
    jupiterObject = new DrawableObject(planetModel, solarShaderProgram, planetModel->count(), GL_TRIANGLES);
    saturnObject = new DrawableObject(planetModel, solarShaderProgram, planetModel->count(), GL_TRIANGLES);
    uranusObject = new DrawableObject(planetModel, solarShaderProgram, planetModel->count(), GL_TRIANGLES);
    neptuneObject = new DrawableObject(planetModel, solarShaderProgram, planetModel->count(), GL_TRIANGLES);

    sunTexture = new Texture("sun.png");
    earthTexture = new Texture("earth.png");
    moonTexture = new Texture("moon.png");
	mercuryTexture = new Texture("mercury.png");
	venusTexture = new Texture("venus.png");
	marsTexture = new Texture("mars.png");
	jupiterTexture = new Texture("jupiter.png");
	saturnTexture = new Texture("saturn.png");
	uranusTexture = new Texture("uranus.png");
	neptuneTexture = new Texture("neptune.png");

    solarSkybox = new Skybox("stars.jpg", "stars.jpg", "stars.jpg", "stars.jpg", "stars.jpg", "stars.jpg");

	models.push_back(planetModel);
    shaderPrograms.push_back(solarShaderProgram);
    objects.push_back(sunObject); objects.push_back(earthObject); objects.push_back(moonObject); objects.push_back(mercuryObject); objects.push_back(venusObject); objects.push_back(marsObject); objects.push_back(jupiterObject); objects.push_back(saturnObject); objects.push_back(uranusObject); objects.push_back(neptuneObject);
    textures.push_back(sunTexture); textures.push_back(earthTexture); textures.push_back(moonTexture); textures.push_back(mercuryTexture); textures.push_back(venusTexture); textures.push_back(marsTexture); textures.push_back(jupiterTexture); textures.push_back(saturnTexture); textures.push_back(uranusTexture); textures.push_back(neptuneTexture);

    camera2->addObserver(solarShaderProgram);

    if(solarShaderProgram) Material(1.0f, 1.0f, 1.0f, 32.0f).applyM(solarShaderProgram);

    if(!pointLight) pointLight = new Light(vec3(0.0f, 0.0f, 0.0f));
    if(lighting) lighting->setPointLight(pointLight);
    if(pointLight && solarShaderProgram) pointLight->addObserver(solarShaderProgram);
}

void Scene::setupScene5()
{
    arcadeloginModel = new Model("login.obj");
    arcadePlainShaderProgram = new ShaderProgram("standard_lit.vert", "forest_arcade.frag");

    arcadePlainObject = new DrawableObject(plainModel, arcadePlainShaderProgram, 6, GL_TRIANGLES);
    arcadeloginObject = new DrawableObject(arcadeloginModel, modelsShaderProgram, arcadeloginModel->count(), GL_TRIANGLES);

    arcadePlainTexture = new Texture("wood.png");
	arcadeloginTexture = new Texture("grass.jpg");

	models.push_back(arcadeloginModel);
    shaderPrograms.push_back(arcadePlainShaderProgram); 
	objects.push_back(arcadePlainObject); objects.push_back(arcadeloginObject);
	textures.push_back(arcadePlainTexture); textures.push_back(arcadeloginTexture);

    camera2->addObserver(arcadePlainShaderProgram);
    if(arcadePlainShaderProgram) Material(vec3(0.35f, 0.20f, 0.08f), 1.0f, 1.0f, 0.0f, 16.0f).applyM(arcadePlainShaderProgram);

    int idx = 0;
    for(int row = 0; row < 6; row++)
    {
        for(int col = 0; col < 6; col++) arcadePositions[idx++] = vec3(-1.2f + col * 0.5f, 0.0f, -1.2f + row * 0.5f);
    }

    srand((unsigned int)time(NULL));
    float base = (float)glfwGetTime();
    for(int i = 0; i < 3; i++)
    {
        while(true)
        {
            int k;
            arcadeShrekPosIndex[i] = rand() % 36;
            for(k = 0; k < i; k++) if(arcadeShrekPosIndex[k] == arcadeShrekPosIndex[i]) break;
            if(k == i) break;
        }
        arcadeShrekLastTime[i] = base + i * 1.0f;
    }
}

void Scene::draw()
{
    if(currentScene == 1)
    {
        mat4 M_triangle(1.0f);
        TransformationComposite TC_triangle;
        TC_triangle.addComponent(new Rotation(0.5f * (float)glfwGetTime(), vec3(0.0f, 0.0f, 1.0f)));
        TC_triangle.addComponent(new Translation(vec3(-0.25f, 0.40f, 0.0f)));
        TC_triangle.apply(M_triangle);

        triangleObject->draw(M_triangle);
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
            mat4 M_light_model(1.0f);
            TransformationComposite TC_light_model;
            TC_light_model.addComponent(new Translation(positions[i]));
            TC_light_model.addComponent(new Scale(vec3(0.20f)));
            TC_light_model.apply(M_light_model);

            modeObjects[i]->draw(M_light_model);
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

        if(skybox)
        {
            if(skyboxPrevOn && !skyboxOn)
            {
                vec3 camPos = activeCamera->getPosition();
                skycubePos = camPos + vec3(0.0f, 1.5f, 0.0f);
            }
            skyboxPrevOn = skyboxOn;

            if(skyboxOn) skybox->applySB(skyboxShaderProgram, true, nullptr, 5);
            else
            {
                mat4 M_skycube(1.0f);
                TransformationComposite TC_skycube;
                TC_skycube.addComponent(new Translation(skycubePos));
                TC_skycube.addComponent(new Scale(vec3(0.80f)));
                TC_skycube.apply(M_skycube);

                skybox->applySB(skyboxShaderProgram, false, &M_skycube, 5);
            }
        }

        if(grassTexture) grassTexture->applyT(plainShaderProgram, 0, 1, 10.0f);
        mat4 M_plain(1.0f);
        TransformationComposite TC_plain;
        TC_plain.addComponent(new Scale(vec3(forestExtent + 0.10f, 1.0f, forestExtent + 0.10f)));
        TC_plain.apply(M_plain);

        plainObject->draw(M_plain);

        for(auto& trees : treeInstances)
        {
            mat4 M_tree(1.0f);
            TransformationComposite TC_tree;
            TC_tree.addComponent(new Translation(vec3(trees.x * forestExtent, 0.0f, -trees.y * forestExtent)));
            TC_tree.addComponent(new Rotation(trees.z, vec3(0, 1, 0)));
            TC_tree.addComponent(new Scale(vec3(trees.w)));
            TC_tree.apply(M_tree);

            treeObject->draw(M_tree);
        }

        for(auto& bushes : bushInstances)
        {
            mat4 M_bush(1.0f);
            TransformationComposite TC_bush;
            TC_bush.addComponent(new Translation(vec3(bushes.x * forestExtent, 0.0f, -bushes.y * forestExtent)));
            TC_bush.addComponent(new Rotation(bushes.z, vec3(0, 1, 0)));
            TC_bush.addComponent(new Scale(vec3(bushes.w)));
            TC_bush.apply(M_bush);

            bushObject->draw(M_bush);
        }

        if(environment && lighting && (forestLightMode == 1 || forestLightMode == 3))
        {
            int n = lighting->fireflyCap;
            if(n > environment->fireflyTotal) n = environment->fireflyTotal;
            if(n > 16) n = 16;

            for(int i = 0; i < n; i++)
            {
                mat4 M_firefly(1.0f);
                TransformationComposite TC_firefly;
                TC_firefly.addComponent(new Translation(environment->fireflyPositions[i]));
                TC_firefly.addComponent(new Scale(vec3(0.04f)));
                TC_firefly.apply(M_firefly);

                fireflyObject->draw(M_firefly);
            }
        }

		if(houseModel) houseModel->applyMdlMtl(modelsShaderProgram);
        if(houseTexture) houseTexture->applyT(modelsShaderProgram, 0, 1, 1.0f);
        mat4 M_house(1.0f);
        TransformationComposite TC_house;
        TC_house.addComponent(new Translation(vec3(0.0f, 0.0f, 0.0f)));
        TC_house.addComponent(new Scale(vec3(0.10f)));
        TC_house.apply(M_house);

        houseObject->draw(M_house);

		if(formulaModel) formulaModel->applyMdlMtl(modelsShaderProgram);
        if(formulaTexture) formulaTexture->applyT(modelsShaderProgram, 0, 1, 1.0f);
        mat4 M_formula(1.0f);
        TransformationComposite TC_formula;
        TC_formula.addComponent(new Translation(vec3(0.90f, 0.005f, 0.0f)));
        TC_formula.addComponent(new Rotation(radians(90.0f), vec3(0, 1, 0)));
        TC_formula.addComponent(new Scale(vec3(0.02f)));
        TC_formula.apply(M_formula);

        formulaObject->draw(M_formula);

		if(cubeModel) cubeModel->applyMdlMtl(modelsShaderProgram);
        if(cubeTexture) cubeTexture->applyT(modelsShaderProgram, 0, 1, 1.0f);
        mat4 M_cube1(1.0f);
        TransformationComposite TC_cube1;
        TC_cube1.addComponent(new Translation(vec3(-0.50f, 0.055f, 1.0f)));
        TC_cube1.addComponent(new Scale(vec3(0.05f)));
        TC_cube1.apply(M_cube1);

        cubeObject->draw(M_cube1);

        mat4 M_cube2(1.0f);
        TransformationComposite TC_cube2;
        TC_cube2.addComponent(new Translation(vec3(-0.65f, 0.055f, 1.0f)));
        TC_cube2.addComponent(new Scale(vec3(0.05f)));
        TC_cube2.apply(M_cube2);

        cubeObject->draw(M_cube2);

        mat4 M_cube3(1.0f);
        TransformationComposite TC_cube3;
        TC_cube3.addComponent(new Translation(vec3(-0.575f, 0.155f, 1.0f)));
        TC_cube3.addComponent(new Scale(vec3(0.05f)));
        TC_cube3.apply(M_cube3);

        cubeObject->draw(M_cube3);

		if(shrekModel) shrekModel->applyMdlMtl(modelsShaderProgram);
        if(shrekTexture) shrekTexture->applyT(modelsShaderProgram, 0, 1, 1.0f);
        mat4 M_shrek(1.0f);
        TransformationComposite TC_shrek;
        TC_shrek.addComponent(new Translation(vec3(-1.2f, 0.0f, 0.20f)));
		TC_shrek.addComponent(new Rotation(radians(120.0f), vec3(0, 1, 0)));
        TC_shrek.addComponent(new Scale(vec3(0.20f)));
        TC_shrek.apply(M_shrek);
        
        shrekObject->draw(M_shrek);

		if(fionaModel) fionaModel->applyMdlMtl(modelsShaderProgram);
        if(fionaTexture) fionaTexture->applyT(modelsShaderProgram, 0, 1, 1.0f);
        mat4 M_fiona(1.0f);
        TransformationComposite TC_fiona;
        TC_fiona.addComponent(new Translation(vec3(1.25f, 0.0f, 0.0f)));
        TC_fiona.addComponent(new Rotation(radians(-90.0f), vec3(0, 1, 0)));
        TC_fiona.addComponent(new Scale(vec3(0.20f)));
        TC_fiona.apply(M_fiona);
        
        fionaObject->draw(M_fiona);

		if(toiletModel) toiletModel->applyMdlMtl(modelsShaderProgram);
        if(toiletTexture) toiletTexture->applyT(modelsShaderProgram, 0, 1, 1.0f);
        mat4 M_toilet(1.0f);
        TransformationComposite TC_toilet;
        TC_toilet.addComponent(new Translation(vec3(-1.0f, 0.0f, -0.25f)));
        TC_toilet.addComponent(new Scale(vec3(0.15f)));
        TC_toilet.apply(M_toilet);
        
        toiletObject->draw(M_toilet);
    }
    else if(currentScene == 4)
    {
        if(pointLight) pointLight->setPosition(vec3(0.0f));
        if(solarSkybox)
        {
            if(skyboxPrevOn && !skyboxOn)
            {
                vec3 camPos = activeCamera->getPosition();
                skycubePos = camPos + vec3(0.0f, 1.5f, 0.0f);
            }
            skyboxPrevOn = skyboxOn;

            if(skyboxOn) solarSkybox->applySB(skyboxShaderProgram, true, nullptr, 5);
            else
            {
                mat4 M_skycube(1.0f);
                TransformationComposite TC_skycube;
                TC_skycube.addComponent(new Translation(skycubePos));
                TC_skycube.addComponent(new Scale(vec3(0.80f)));
                TC_skycube.apply(M_skycube);

                solarSkybox->applySB(skyboxShaderProgram, false, &M_skycube, 5);
            }
        }

        vec3 mercuryPos = vec3(0.8f * cos(1.8f * (float)glfwGetTime()), 0.8f * sin(1.8f * (float)glfwGetTime()), 0.0f);
        vec3 venusPos = vec3(1.4f * cos(0.35f * (float)glfwGetTime()), 1.4f * sin(0.35f * (float)glfwGetTime()), 0.0f);
        vec3 earthPos = vec3(2.0f * cos(0.50f * (float)glfwGetTime()), 2.0f * sin(0.50f * (float)glfwGetTime()), 0.0f);
        vec3 marsPos = vec3(2.6f * cos(0.25f * (float)glfwGetTime()), 2.6f * sin(0.25f * (float)glfwGetTime()), 0.0f);
        vec3 jupiterPos = vec3(3.2f * cos(0.15f * (float)glfwGetTime()), 3.2f * sin(0.15f * (float)glfwGetTime()), 0.0f);
        vec3 saturnPos = vec3(3.8f * cos(0.12f * (float)glfwGetTime()), 3.8f * sin(0.12f * (float)glfwGetTime()), 0.0f);
        vec3 uranusPos = vec3(4.4f * cos(0.08f * (float)glfwGetTime()), 4.4f * sin(0.08f * (float)glfwGetTime()), 0.0f);
        vec3 neptunePos = vec3(5.0f * cos(0.06f * (float)glfwGetTime()), 5.0f * sin(0.06f * (float)glfwGetTime()), 0.0f);

        vec3 colors[9] = {vec3(1.0f, 0.93f, 0.30f), vec3(0.65f), vec3(0.9f), vec3(0.20f, 0.45f, 0.90f), vec3(1.0f, 0.5f, 0.0f), vec3(0.8f, 0.3f, 0.0f), vec3(0.5f), vec3(0.2f, 0.4f, 0.7f), vec3(0.0f, 0.0f, 1.0f)};
        vec3 scales[9] = {vec3(0.5f), vec3(0.15f), vec3(0.12f), vec3(0.1f), vec3(0.11f), vec3(0.24f), vec3(0.30f), vec3(0.18f), vec3(0.16f)};
        vec3 positions[9] = {vec3(0.0f), mercuryPos, venusPos, earthPos, marsPos, jupiterPos, saturnPos, uranusPos, neptunePos};
        float selfRot[9] = {0.4f, 1.0f, 0.8f, 1.2f, 1.0f, 1.8f, 1.6f, 1.3f, 1.1f};
        float emissive[10] = {0.8f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

        DrawableObject* solarObjects[9] = {sunObject, mercuryObject, venusObject, earthObject, marsObject, jupiterObject, saturnObject, uranusObject, neptuneObject};
        Texture* solarTextures[9] = {sunTexture, mercuryTexture, venusTexture, earthTexture, marsTexture, jupiterTexture, saturnTexture, uranusTexture, neptuneTexture};
        for(int i = 0; i < 9; i++)
        {
            mat4 M_solar(1.0f);
            TransformationComposite TC_solar;
            TC_solar.addComponent(new Translation(positions[i]));
            TC_solar.addComponent(new Rotation(selfRot[i] * (float)glfwGetTime(), vec3(0.0f, 1.0f, 0.0f)));
            TC_solar.addComponent(new Scale(scales[i]));
            TC_solar.apply(M_solar);

            if(lighting) lighting->applyL(solarShaderProgram, 4, 0, 0, nullptr, 0, vec3(0.0f), vec3(0.0f), false, &colors[i], &emissive[i]);
            if(solarTextures[i]) solarTextures[i]->applyT(solarShaderProgram, 0, 1, 1.0f);
            solarObjects[i]->draw(M_solar);
        }

        if(moonObject && moonTexture)
        {
            int moonCounts[6] = {1, 2, 4, 7, 5, 3};
            vec3 moonColor = vec3(0.65f), planetForMoons[6] = {earthPos, marsPos, jupiterPos, saturnPos, uranusPos, neptunePos};
            float moonEmissive = 0.0f, moonRadius[6] = {0.20f, 0.25f, 0.35f, 0.38f, 0.30f, 0.28f};

            if(lighting) lighting->applyL(solarShaderProgram, 4, 0, 0, nullptr, 0, vec3(0.0f), vec3(0.0f), false, &moonColor, &moonEmissive);
            moonTexture->applyT(solarShaderProgram, 0, 1, 1.0f);

            for(int plt = 0; plt < 6; plt++)
            {
                float step = 2.0f * 3.14f / (float)moonCounts[plt];

                for(int k = 0; k < moonCounts[plt]; k++)
                {
                    float angle = (float)glfwGetTime() + step * k;
                    vec3 offset = vec3(cos(angle) * moonRadius[plt], sin(angle) * moonRadius[plt], 0.0f);

                    mat4 M_moon(1.0f);
                    TransformationComposite TC_moon;
                    TC_moon.addComponent(new Translation(planetForMoons[plt] + offset));
                    TC_moon.addComponent(new Rotation(2.0f * (float)glfwGetTime(), vec3(0.0f, 1.0f, 0.0f)));
                    TC_moon.addComponent(new Scale(vec3(0.03f)));
                    TC_moon.apply(M_moon);

                    moonObject->draw(M_moon);
                }
            }
        }
    }
    else if(currentScene == 5)
    {
        float t = (float)glfwGetTime();
        for(int i = 0; i < 3; i++)
        {
            if(t - arcadeShrekLastTime[i] >= 3.0f)
            {
                arcadeShrekLastTime[i] += 3.0f;
                int oldIndex = arcadeShrekPosIndex[i], newIndex;

                while(true)
                {
                    int k;
                    newIndex = rand() % 36;
                    if(newIndex == oldIndex) continue;
                    
                    for(k = 0; k < 3; k++) if(k != i && arcadeShrekPosIndex[k] == newIndex) break;
                    if(k == 3) break;
                }
                arcadeShrekPosIndex[i] = newIndex;
            }
        }

        if(arcadePlainTexture) arcadePlainTexture->applyT(arcadePlainShaderProgram, 0, 1, 10.0f);
        mat4 M_plain_Arcade(1.0f);
        TransformationComposite TC_plain_Arcade;
        TC_plain_Arcade.addComponent(new Scale(vec3(2.0f, 1.0f, 2.0f)));
        TC_plain_Arcade.apply(M_plain_Arcade);

        arcadePlainObject->draw(M_plain_Arcade);

        vec3 path[3] = {vec3(-1.5f, 0.8f, -2.5f), vec3(0.0f, 0.8f, -1.0f), vec3(1.5f, 0.8f, -2.5f)};
        float s = t * 0.3f;
        int whole = (int)s, seg = whole % 3;
        float a = s - (float)whole;

        vec3 p0 = path[seg], p1 = path[(seg + 1) % 3];
        vec3 loginPos = p0 + (p1 - p0) * a;

        if(arcadeloginModel) arcadeloginModel->applyMdlMtl(modelsShaderProgram);
        if(arcadeloginTexture) arcadeloginTexture->applyT(modelsShaderProgram, 0, 1, 1.0f);

        mat4 M_login_Arcade(1.0f);
        TransformationComposite TC_login_Arcade;
        TC_login_Arcade.addComponent(new Translation(loginPos));
        TC_login_Arcade.addComponent(new Rotation(radians(180.0f), vec3(0, 1, 0)));
        TC_login_Arcade.addComponent(new Scale(vec3(1.2f)));
        TC_login_Arcade.apply(M_login_Arcade);

        arcadeloginObject->draw(M_login_Arcade);

        if(shrekModel) shrekModel->applyMdlMtl(modelsShaderProgram);
        if(shrekTexture) shrekTexture->applyT(modelsShaderProgram, 0, 1, 1.0f);
        for(int i = 0; i < 3; i++)
        {
            int idx = (arcadeShrekPosIndex[i] < 0 || arcadeShrekPosIndex[i] >= 36) ? 0 : arcadeShrekPosIndex[i];
            vec3 shrekPos = arcadePositions[idx];

            mat4 M_shrek_Arcade(1.0f);
            TransformationComposite TC_shrek_Arcade;
            TC_shrek_Arcade.addComponent(new Translation(shrekPos));
            TC_shrek_Arcade.addComponent(new Scale(vec3(0.25f)));
            TC_shrek_Arcade.apply(M_shrek_Arcade);

            shrekObject->draw(M_shrek_Arcade);
        }
    }
}