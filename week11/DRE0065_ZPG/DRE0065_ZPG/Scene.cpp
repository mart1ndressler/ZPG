#include "Scene.h"
#include "sphere.h"
#include "tree.h"
#include "bushes.h"
#include "plain.h"
#include "triangle.h"

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
	setupScene6();
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
    if(currentScene == 6)
    {
        bezierControlPoints.clear();
        if(isNewScene) fprintf(stderr, "[BEZIER] Control points cleared on scene change\n");
    }

    if(currentScene == 2)
    {
        scene2Mode = 4;
        scene2Lights = 1;

        activeCamera = camera1;
        camera1->reset(vec3(0.0f, 0.0f, 1.5f), -90.0f, 0.0f);
    }
    else if(currentScene == 3 || currentScene == 4 || currentScene == 5 || currentScene == 6)
    {
        activeCamera = camera2;
        camera2->reset(vec3(0.0f, 3.0f, 7.0f), -90.0f, -20.0f);

        forestLightMode = 0;
        flashlightOn = false;

        (currentScene == 3 || currentScene == 4) ? skyboxOn = true : skyboxOn = false;
    }
    transformMode = 0;
    setTransformMode(transformMode);
    camera1->setFov(60.0f);
}

int Scene::getScene()
{
    return currentScene;
}

void Scene::updateCamera(GLFWwindow* window, float deltaTime, double xpos, double ypos, bool rightPressed)
{
    if(currentScene == 2 || currentScene == 3 || currentScene == 4 || currentScene == 5 || currentScene == 6)
    {
        //float speedFactor = (transformMode == 2) ? 1.0f / 20.0f : 1.0f;
        activeCamera->processKeyboard(window, deltaTime/* * speedFactor */);
        activeCamera->processMouse(xpos, ypos, rightPressed);
    }
}

void Scene::setAspect(float a)
{
    camera1->setAspect(a);
    camera2->setAspect(a);
}

void Scene::setFov(float f)
{
    activeCamera->setFov(f);
}

void Scene::handleClick(int mouseX, int mouseY, int width, int height)
{
    GLint x = mouseX, y = mouseY, newY = height - y;
    GLbyte color[4];
    GLfloat depth;
    GLuint index;
	float t = (float)glfwGetTime();

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
        else if(picked == shrekObject) objectName = "Shrek (Sc.3)";
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
        else if(picked == arcadePlainObject) objectName = "Arcade-Plain (Sc.5)";
        else if(picked == arcadeloginObject) objectName = "Login (Sc.5)";
		else if(picked == arcadeShrekObject) objectName = "Arcade-Shrek (Sc.5)";
        else if(picked == bezierPlainObject) objectName = "Bezier-Plain (Sc.6)";
		else if(picked == bezierFormulaObject) objectName = "Bezier-F1 (Sc.6)";

        if(picked == arcadeShrekObject)
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
                arcadeShrekLastTime[hitIdx] = t;

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
        if(picked == bezierPlainObject)
        {
            vec3 screenPoint((float)x, (float)newY, depth);
            vec4 viewport(0.0f, 0.0f, (float)width, (float)height);

            vec3 worldPos = unProject(screenPoint, activeCamera->getViewMatrix(), activeCamera->getProjectionMatrix(), viewport);
            bezierControlPoints.push_back(worldPos);
            fprintf(stderr, "[BEZIER] Added control point #%d at [%f, %f, %f]\n", (int)bezierControlPoints.size(), worldPos.x, worldPos.y, worldPos.z);
        }
    }
    if(picked != arcadeShrekObject && picked != bezierPlainObject) fprintf(stderr, "[PICK] Clicked pixel(%d, %d -> newY=%d), ""color=%02hhx%02hhx%02hhx%02hhx, depth=%f, stencil index=%u, object=%s\n", x, y, newY, color[0], color[1], color[2], color[3], depth, index, objectName);
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

void Scene::evalBezierSegment(const vec3& p0, const vec3& p1, const vec3& p2, const vec3& p3, float t, vec3& outPos, vec3& outTangent)
{
    const mat4 A(vec4(-1.0f, 3.0f, -3.0f, 1.0f), vec4(3.0f, -6.0f, 3.0f, 0.0f), vec4(-3.0f, 3.0f, 0.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 0.0f));
    mat4x3 B(p0, p1, p2, p3);

    vec4 params(t * t * t, t * t, t, 1.0f), dparams(3.0f * t * t, 2.0f * t, 1.0f, 0.0f);
    outPos = params * A * transpose(B);
    outTangent = dparams * A * transpose(B);
}

void Scene::setupScene1()
{
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

    Material(vec3(0.7f), 1.0f, 1.0f, 0.0f, 16.0f).applyM(lambertShaderProgram);
    Material(vec3(0.7f), 1.0f, 1.0f, 1.0f, 16.0f).applyM(phongShaderProgram);
    Material(vec3(0.7f), 1.0f, 1.0f, 1.0f, 128.0f).applyM(blinnShaderProgram);

    if(!pointLight) pointLight = new Light(vec3(0.0f, 0.0f, 0.0f));
    lighting->setPointLight(pointLight);
    pointLight->addObserver(constantShaderProgram);
    pointLight->addObserver(lambertShaderProgram);
    pointLight->addObserver(phongShaderProgram);
    pointLight->addObserver(blinnShaderProgram);
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

    plainShaderProgram = new ShaderProgram("standard_lit.vert", "lambert.frag");
    treeShaderProgram = new ShaderProgram("standard_lit.vert", "lambert.frag");
    bushShaderProgram = new ShaderProgram("standard_lit.vert", "lambert.frag");
    forestShaderProgram = new ShaderProgram("standard_lit.vert", "lambert.frag");
    fireflyShaderProgram = new ShaderProgram("standard_lit.vert", "constant.frag");

    plainObject = new DrawableObject(plainModel, plainShaderProgram, 6, GL_TRIANGLES);
    treeObject = new DrawableObject(treeModel, treeShaderProgram, 92814, GL_TRIANGLES);
    bushObject = new DrawableObject(bushModel, bushShaderProgram, 8730, GL_TRIANGLES);
    houseObject = new DrawableObject(houseModel, forestShaderProgram, houseModel->count(), GL_TRIANGLES);
    formulaObject = new DrawableObject(formulaModel, forestShaderProgram, formulaModel->count(), GL_TRIANGLES);
    cubeObject = new DrawableObject(cubeModel, forestShaderProgram, cubeModel->count(), GL_TRIANGLES);
    shrekObject = new DrawableObject(shrekModel, forestShaderProgram, shrekModel->count(), GL_TRIANGLES);
    toiletObject = new DrawableObject(toiletModel, forestShaderProgram, toiletModel->count(), GL_TRIANGLES);
    fionaObject = new DrawableObject(fionaModel, forestShaderProgram, fionaModel->count(), GL_TRIANGLES);
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
    shaderPrograms.push_back(plainShaderProgram); shaderPrograms.push_back(treeShaderProgram); shaderPrograms.push_back(bushShaderProgram); shaderPrograms.push_back(forestShaderProgram); shaderPrograms.push_back(fireflyShaderProgram);
	objects.push_back(plainObject); objects.push_back(treeObject); objects.push_back(bushObject); objects.push_back(houseObject), objects.push_back(formulaObject); objects.push_back(cubeObject); objects.push_back(shrekObject); objects.push_back(toiletObject); objects.push_back(fionaObject); objects.push_back(fireflyObject);
	textures.push_back(grassTexture); textures.push_back(houseTexture); textures.push_back(formulaTexture); textures.push_back(cubeTexture); textures.push_back(shrekTexture); textures.push_back(toiletTexture); textures.push_back(fionaTexture);

	camera2->addObserver(plainShaderProgram);
	camera2->addObserver(treeShaderProgram);
	camera2->addObserver(bushShaderProgram);
    camera2->addObserver(fireflyShaderProgram);
	camera2->addObserver(forestShaderProgram);

    Material(1.0f, 1.0f, 0.0f, 16.0f).applyM(plainShaderProgram);
    Material(vec3(0.06f, 0.18f, 0.08f), 1.0f, 1.0f, 0.0f, 16.0f).applyM(treeShaderProgram);
    Material(vec3(0.20f, 0.65f, 0.28f), 1.0f, 1.0f, 0.0f, 16.0f).applyM(bushShaderProgram);

    srand((unsigned int)time(NULL));
    treeInstances.clear(); bushInstances.clear();

    environment->initForest(0.55f, treeInstances, bushInstances);
}

void Scene::setupScene4()
{
	planetModel = new Model("planet.obj");
    solarShaderProgram = new ShaderProgram("standard_lit.vert", "phong.frag");

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
    Material(1.0f, 1.0f, 1.0f, 16.0f).applyM(solarShaderProgram);

    if(!pointLight) pointLight = new Light(vec3(0.0f, 0.0f, 0.0f));
    lighting->setPointLight(pointLight);
    pointLight->addObserver(solarShaderProgram);
}

void Scene::setupScene5()
{
    arcadeloginModel = new Model("login_ZPG.obj");
    arcadeShaderProgram = new ShaderProgram("standard_lit.vert", "lambert.frag");

    arcadePlainObject = new DrawableObject(plainModel, arcadeShaderProgram, 6, GL_TRIANGLES);
    arcadeloginObject = new DrawableObject(arcadeloginModel, arcadeShaderProgram, arcadeloginModel->count(), GL_TRIANGLES);
	arcadeShrekObject = new DrawableObject(shrekModel, arcadeShaderProgram, shrekModel->count(), GL_TRIANGLES);

    arcadePlainTexture = new Texture("wood.png");
	arcadeloginTexture = new Texture("grass.jpg");

	models.push_back(arcadeloginModel);
    shaderPrograms.push_back(arcadeShaderProgram);
	objects.push_back(arcadePlainObject); objects.push_back(arcadeloginObject); objects.push_back(arcadeShrekObject);
	textures.push_back(arcadePlainTexture); textures.push_back(arcadeloginTexture);

    camera2->addObserver(arcadeShaderProgram);
    Material(1.0f, 1.0f, 0.0f, 16.0f).applyM(arcadeShaderProgram);

    if(!pointLight) pointLight = new Light(vec3(0.0f, 3.0f, 0.0f));
    lighting->setPointLight(pointLight);
    pointLight->addObserver(arcadeShaderProgram);

    int rows = 6, cols = 6;
    environment->initArcadeGrid(arcadePositions, rows, cols, vec3(-1.2f, 0.0f, -1.2f), 0.5f);

    srand((unsigned int)time(NULL));
    float base = (float)glfwGetTime();
    environment->initArcadeShreks(arcadeShrekPosIndex, arcadeShrekLastTime, 3, rows * cols, base);
}

void Scene::setupScene6()
{
    bezierShaderProgram = new ShaderProgram("standard_lit.vert", "lambert.frag");

    bezierPlainObject = new DrawableObject(plainModel, bezierShaderProgram, 6, GL_TRIANGLES);
    bezierFormulaObject = new DrawableObject(formulaModel, bezierShaderProgram, formulaModel->count(), GL_TRIANGLES);

    shaderPrograms.push_back(bezierShaderProgram);
    objects.push_back(bezierPlainObject); objects.push_back(bezierFormulaObject);

    camera2->addObserver(bezierShaderProgram);
    Material(1.0f, 1.0f, 0.0f, 16.0f).applyM(bezierShaderProgram);

    if(!pointLight) pointLight = new Light(vec3(0.0f, 3.0f, 0.0f));
    lighting->setPointLight(pointLight);
    pointLight->addObserver(bezierShaderProgram);
}

void Scene::draw()
{
    float t = (float)glfwGetTime();
    if(currentScene == 1)
    {
        mat4 M_triangle(1.0f);
        TransformationComposite TC_triangle;
        TC_triangle.addComponent(new Rotation(0.5f * t, vec3(0.0f, 0.0f, 1.0f)));
        TC_triangle.addComponent(new Translation(vec3(-0.25f, 0.40f, 0.0f)));
        TC_triangle.apply(M_triangle);

        triangleObject->draw(M_triangle);
    }
    else if(currentScene == 2)
    {
        lighting->applyL(constantShaderProgram, currentScene, scene2Lights);
        lighting->applyL(lambertShaderProgram, currentScene, scene2Lights);
        lighting->applyL(phongShaderProgram, currentScene, scene2Lights);
        lighting->applyL(blinnShaderProgram, currentScene, scene2Lights);

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
        environment->updateFireflies();
        lighting->applyL(plainShaderProgram, currentScene, 0, forestLightMode, environment->fireflyPositions, environment->fireflyTotal, activeCamera->getPosition(), activeCamera->getFront(), flashlightOn);
        lighting->applyL(treeShaderProgram, currentScene, 0, forestLightMode, environment->fireflyPositions, environment->fireflyTotal, activeCamera->getPosition(), activeCamera->getFront(), flashlightOn);
        lighting->applyL(bushShaderProgram, currentScene, 0, forestLightMode, environment->fireflyPositions, environment->fireflyTotal, activeCamera->getPosition(), activeCamera->getFront(), flashlightOn);
        lighting->applyL(forestShaderProgram, currentScene, 0, forestLightMode, environment->fireflyPositions, environment->fireflyTotal, activeCamera->getPosition(), activeCamera->getFront(), flashlightOn);

        if(skyboxPrevOn && !skyboxOn)
        {
            vec3 camPos = activeCamera->getPosition();
            skycubePos = camPos + vec3(0.0f, 1.5f, 0.0f);
        }
        skyboxPrevOn = skyboxOn;

        if(skyboxOn) skybox->applySB(forestShaderProgram, true, nullptr, 5);
        else
        {
            mat4 M_skycube(1.0f);
            TransformationComposite TC_skycube;
            TC_skycube.addComponent(new Translation(skycubePos));
            TC_skycube.addComponent(new Scale(vec3(0.80f)));
            TC_skycube.apply(M_skycube);

            skybox->applySB(forestShaderProgram, false, &M_skycube, 5);
        }

        grassTexture->applyT(plainShaderProgram, 0, 1, 10.0f);
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

        if(forestLightMode == 1 || forestLightMode == 3)
        {
            int n = std::min(lighting->fireflyCap, std::min(environment->fireflyTotal, 16));
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

		houseModel->applyMdlMtl(forestShaderProgram);
        houseTexture->applyT(forestShaderProgram, 0, 1, 1.0f);
        mat4 M_house(1.0f);
        TransformationComposite TC_house;
        TC_house.addComponent(new Translation(vec3(0.0f, 0.0f, 0.0f)));
        TC_house.addComponent(new Scale(vec3(0.10f)));
        TC_house.apply(M_house);

        houseObject->draw(M_house);

		formulaModel->applyMdlMtl(forestShaderProgram);
        formulaTexture->applyT(forestShaderProgram, 0, 1, 1.0f);
        mat4 M_formula(1.0f);
        TransformationComposite TC_formula;
        TC_formula.addComponent(new Translation(vec3(0.90f, 0.005f, 0.0f)));
        TC_formula.addComponent(new Rotation(radians(90.0f), vec3(0, 1, 0)));
        TC_formula.addComponent(new Scale(vec3(0.02f)));
        TC_formula.apply(M_formula);

        formulaObject->draw(M_formula);

		cubeModel->applyMdlMtl(forestShaderProgram);
        cubeTexture->applyT(forestShaderProgram, 0, 1, 1.0f);
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

		shrekModel->applyMdlMtl(forestShaderProgram);
        shrekTexture->applyT(forestShaderProgram, 0, 1, 1.0f);
        mat4 M_shrek(1.0f);
        TransformationComposite TC_shrek;
        TC_shrek.addComponent(new Translation(vec3(-1.2f, 0.0f, 0.20f)));
		TC_shrek.addComponent(new Rotation(radians(120.0f), vec3(0, 1, 0)));
        TC_shrek.addComponent(new Scale(vec3(0.20f)));
        TC_shrek.apply(M_shrek);
        
        shrekObject->draw(M_shrek);

		fionaModel->applyMdlMtl(forestShaderProgram);
        fionaTexture->applyT(forestShaderProgram, 0, 1, 1.0f);
        mat4 M_fiona(1.0f);
        TransformationComposite TC_fiona;
        TC_fiona.addComponent(new Translation(vec3(1.25f, 0.0f, 0.0f)));
        TC_fiona.addComponent(new Rotation(radians(-90.0f), vec3(0, 1, 0)));
        TC_fiona.addComponent(new Scale(vec3(0.20f)));
        TC_fiona.apply(M_fiona);
        
        fionaObject->draw(M_fiona);

		toiletModel->applyMdlMtl(forestShaderProgram);
        toiletTexture->applyT(forestShaderProgram, 0, 1, 1.0f);
        mat4 M_toilet(1.0f);
        TransformationComposite TC_toilet;
        TC_toilet.addComponent(new Translation(vec3(-1.0f, 0.0f, -0.25f)));
        TC_toilet.addComponent(new Scale(vec3(0.15f)));
        TC_toilet.apply(M_toilet);
        
        toiletObject->draw(M_toilet);
    }
    else if(currentScene == 4)
    {
        pointLight->setPosition(vec3(0.0f));
        if(skyboxPrevOn && !skyboxOn)
        {
            vec3 camPos = activeCamera->getPosition();
            skycubePos = camPos + vec3(0.0f, 1.5f, 0.0f);
        }
        skyboxPrevOn = skyboxOn;

        if(skyboxOn) solarSkybox->applySB(forestShaderProgram, true, nullptr, 5);
        else
        {
            mat4 M_skycube(1.0f);
            TransformationComposite TC_skycube;
            TC_skycube.addComponent(new Translation(skycubePos));
            TC_skycube.addComponent(new Scale(vec3(0.80f)));
            TC_skycube.apply(M_skycube);

            solarSkybox->applySB(forestShaderProgram, false, &M_skycube, 5);
        }

        vec3 mercuryPos = vec3(0.8f * cos(1.8f * t), 0.0f, 0.8f * sin(1.8f * t));
        vec3 venusPos = vec3(1.5f * cos(0.35f * t), 0.0f, 1.5f * sin(0.35f * t));
        vec3 earthPos = vec3(2.2f * cos(0.50f * t), 0.0f, 2.2f * sin(0.50f * t));
        vec3 marsPos = vec3(2.9f * cos(0.25f * t), 0.0f, 2.9f * sin(0.25f * t));
        vec3 jupiterPos = vec3(3.6f * cos(0.15f * t), 0.0f, 3.6f * sin(0.15f * t));
        vec3 saturnPos = vec3(4.3f * cos(0.12f * t), 0.0f, 4.3f * sin(0.12f * t));
        vec3 uranusPos = vec3(5.0f * cos(0.08f * t), 0.0f, 5.0f * sin(0.08f * t));
        vec3 neptunePos = vec3(5.7f * cos(0.06f * t), 0.0f, 5.7f * sin(0.06f * t));

        vec3 colors[9] = {vec3(1.0f, 0.93f, 0.30f), vec3(0.65f), vec3(0.9f), vec3(0.20f, 0.45f, 0.90f), vec3(1.0f, 0.5f, 0.0f), vec3(0.8f, 0.3f, 0.0f), vec3(0.5f), vec3(0.2f, 0.4f, 0.7f), vec3(0.0f, 0.0f, 1.0f)};
        vec3 scales[9] = {vec3(0.5f), vec3(0.15f), vec3(0.12f), vec3(0.1f), vec3(0.11f), vec3(0.24f), vec3(0.30f), vec3(0.18f), vec3(0.16f)};
        vec3 positions[9] = {vec3(0.0f), mercuryPos, venusPos, earthPos, marsPos, jupiterPos, saturnPos, uranusPos, neptunePos}, planetSelfAxis[9];
        float selfRot[9] = {0.4f, 1.0f, 0.8f, 1.2f, 1.0f, 1.8f, 1.6f, 1.3f, 1.1f}, emissive[10] = {};

        for(int i = 0; i < 9; i++) planetSelfAxis[i] = vec3(0.0f, 1.0f, 0.0f);
        emissive[0] = 0.8f;

        DrawableObject* solarObjects[9] = {sunObject, mercuryObject, venusObject, earthObject, marsObject, jupiterObject, saturnObject, uranusObject, neptuneObject};
        Texture* solarTextures[9] = {sunTexture, mercuryTexture, venusTexture, earthTexture, marsTexture, jupiterTexture, saturnTexture, uranusTexture, neptuneTexture};
        for(int i = 0; i < 9; i++)
        {
            mat4 M_solar(1.0f);
            TransformationComposite TC_solar;
            TC_solar.addComponent(new Translation(positions[i]));
            TC_solar.addComponent(new Rotation(selfRot[i] * t, planetSelfAxis[i]));
            TC_solar.addComponent(new Scale(scales[i]));
            TC_solar.apply(M_solar);

            lighting->applyL(solarShaderProgram, 4, 0, 0, nullptr, 0, vec3(0.0f), vec3(0.0f), false, &colors[i], &emissive[i]);
            if(solarTextures[i]) solarTextures[i]->applyT(solarShaderProgram, 0, 1, 1.0f);
            solarObjects[i]->draw(M_solar);
        }

        int moonCounts[6] = {1, 2, 4, 7, 5, 3};
        vec3 moonColor = vec3(0.65f), planetForMoons[6] = {earthPos, marsPos, jupiterPos, saturnPos, uranusPos, neptunePos}, moonSelfAxis[6];
        float moonEmissive = 0.0f, moonRadius[6] = {0.20f, 0.25f, 0.35f, 0.38f, 0.30f, 0.28f};

        for(int i = 0; i < 6; i++) moonSelfAxis[i] = vec3(0.0f, 1.0f, 0.0f);

        lighting->applyL(solarShaderProgram, 4, 0, 0, nullptr, 0, vec3(0.0f), vec3(0.0f), false, &moonColor, &moonEmissive);
        moonTexture->applyT(solarShaderProgram, 0, 1, 1.0f);
        for(int plt = 0; plt < 6; plt++)
        {
            float step = 2.0f * 3.14f / (float)moonCounts[plt];
            for(int k = 0; k < moonCounts[plt]; k++)
            {
                float angle = t + step * k;
                vec3 offset = vec3(cos(angle) * moonRadius[plt], 0.0f, sin(angle) * moonRadius[plt]);

                mat4 M_moon(1.0f);
                TransformationComposite TC_moon;
                TC_moon.addComponent(new Translation(planetForMoons[plt] + offset));
                TC_moon.addComponent(new Rotation(2.0f * t, moonSelfAxis[plt]));
                TC_moon.addComponent(new Scale(vec3(0.03f)));
                TC_moon.apply(M_moon);

                moonObject->draw(M_moon);
            }
        }
    }
    else if(currentScene == 5)
    {
        pointLight->setPosition(vec3(0.0f, 3.0f, 0.0f));
        lighting->applyL(arcadeShaderProgram, 5);

        environment->updateArcadeShreks(t, arcadeShrekPosIndex, arcadeShrekLastTime, 3, 36, 3.0f);

        arcadePlainTexture->applyT(arcadeShaderProgram, 0, 1, 10.0f);
        mat4 M_plain_Arcade(1.0f);
        TransformationComposite TC_plain_Arcade;
        TC_plain_Arcade.addComponent(new Scale(vec3(2.0f, 1.0f, 2.0f)));
        TC_plain_Arcade.apply(M_plain_Arcade);

        arcadePlainObject->draw(M_plain_Arcade);

        arcadeloginModel->applyMdlMtl(arcadeShaderProgram);
        arcadeloginTexture->applyT(arcadeShaderProgram, 0, 1, 1.0f);
        mat4 M_login_Arcade(1.0f);
        TransformationComposite TC_login_Arcade;
        TC_login_Arcade.addComponent(new Translation(vec3(-1.1f, 0.8f, -1.6f)));
        TC_login_Arcade.addComponent(new Scale(vec3(0.6f)));
        TC_login_Arcade.apply(M_login_Arcade);

        arcadeloginObject->draw(M_login_Arcade);

        shrekModel->applyMdlMtl(arcadeShaderProgram);
        shrekTexture->applyT(arcadeShaderProgram, 0, 1, 1.0f);
        for(int i = 0; i < 3; i++)
        {
            int idx = (arcadeShrekPosIndex[i] < 0 || arcadeShrekPosIndex[i] >= 36) ? 0 : arcadeShrekPosIndex[i];
            vec3 shrekPos = arcadePositions[idx];

            mat4 M_shrek_Arcade(1.0f);
            TransformationComposite TC_shrek_Arcade;
            TC_shrek_Arcade.addComponent(new Translation(shrekPos));
            TC_shrek_Arcade.addComponent(new Scale(vec3(0.25f)));
            TC_shrek_Arcade.apply(M_shrek_Arcade);

            arcadeShrekObject->draw(M_shrek_Arcade);
        }
    }
    else if(currentScene == 6)
    {
        pointLight->setPosition(vec3(0.0f, 3.0f, 0.0f));
        lighting->applyL(bezierShaderProgram, 6);

        grassTexture->applyT(bezierShaderProgram, 0, 1, 10.0f);
        mat4 M_plain_bezier(1.0f);
        TransformationComposite TC_plain_bezier;
        TC_plain_bezier.addComponent(new Scale(vec3(2.0f, 1.0f, 2.0f)));
        TC_plain_bezier.apply(M_plain_bezier);

        bezierPlainObject->draw(M_plain_bezier);

        vec3 pos(0.0f), tangent(0.0f, 0.0f, 1.0f);
        int segments = ((int)bezierControlPoints.size() - 1) / 3;
        if(segments > 0)
        {
            float s = fmod(t * bezierSpeed, segments * 2.0f);
            if(s > segments) s = 2.0f * segments - s;

            int segIndex = (int)s;
            if(segIndex >= segments) segIndex = segments - 1;

            float localT = s - segIndex;
            const vec3* p = &bezierControlPoints[segIndex * 3];
            evalBezierSegment(p[0], p[1], p[2], p[3], localT, pos, tangent);
        }
        pos.y += 0.02f;

        formulaModel->applyMdlMtl(bezierShaderProgram);
        formulaTexture->applyT(bezierShaderProgram, 0, 1, 1.0f);
        mat4 M_formula_bezier(1.0f);
        TransformationComposite TC_formula_bezier;
        TC_formula_bezier.addComponent(new Translation(pos));

        vec3 dir = vec3(tangent.x, 0.0f, tangent.z);
        if(length(dir) > 0.0f)
        {
            dir = normalize(dir);
            float yaw = atan2(dir.x, dir.z);
            TC_formula_bezier.addComponent(new Rotation(yaw + radians(90.0f), vec3(0, 1, 0)));
        }
        else TC_formula_bezier.addComponent(new Rotation(radians(90.0f), vec3(0, 1, 0)));
        TC_formula_bezier.addComponent(new Scale(vec3(0.02f)));
        TC_formula_bezier.apply(M_formula_bezier);

        bezierFormulaObject->draw(M_formula_bezier);
    }
}