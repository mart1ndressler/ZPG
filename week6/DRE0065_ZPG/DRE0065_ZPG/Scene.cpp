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
}

void Scene::setScene(int id)
{
    int prevScene = currentScene;
    bool isSameScene = (currentScene == id);
    currentScene = id;

    if(!isSameScene) fprintf(stderr, "Switching to scene %d\n", currentScene);
    if(prevScene == 2 && currentScene != 2)
    {
        scene2Mode = 4;
        scene2Lights = 1;
    }
    if(prevScene == 3 && currentScene != 3) fireflyReady = false;

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
    else if(currentScene == 4)
    {
        activeCamera = camera2;
        camera2->reset(vec3(0.0f, 3.0f, 7.0f), -90.0f, -20.0f);
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

void Scene::applyLight(ShaderProgram* p)
{
    if(!p) return;
    p->use();
    if(pointLight) p->set("lightPosition", pointLight->getPosition());

    if(currentScene == 2)
    {
        vec3 lights[5] = {vec3(0.0f, 0.0f, 0.0f), vec3(0.40f, 0.40f, 0.0f), vec3(-0.40f, 0.40f, 0.0f), vec3(-0.40f, -0.40f, 0.0f), vec3(0.40f, -0.40f, 0.0f)};
        scene2Lights = clamp(scene2Lights, 0, 5);
        p->set("numberOfLights", scene2Lights);

        vec4 kd(1.0f), ks(1.0f);
        for(int i = 0; i < 5; i++)
        {
            char n[32];
            snprintf(n, sizeof(n), "uLights[%d].position", i); p->set(n, vec4(lights[i], 1.0f));
            snprintf(n, sizeof(n), "uLights[%d].diffuse", i); p->set(n, kd);
            snprintf(n, sizeof(n), "uLights[%d].specular", i); p->set(n, ks);
        }

        if(scene2Lights > 0) p->set("lightPosition", lights[0]);
    }
    else if(currentScene == 3)
    {
        updateFireflies();

        vec4 kd(1.0f, 0.95f, 0.6f, 1.0f), ks(1.0f);
        vec3 att(1.0f, 0.7f, 1.8f);
        p->set("numberOfLights", 5);

        for(int i = 0; i < 5; i++)
        {
            char n[32];
            snprintf(n, sizeof(n), "uLights[%d].position", i); p->set(n, vec4(fireflyPos[i], 1.0f));
            snprintf(n, sizeof(n), "uLights[%d].diffuse", i); p->set(n, kd);
            snprintf(n, sizeof(n), "uLights[%d].specular", i); p->set(n, ks);
            snprintf(n, sizeof(n), "uLights[%d].atten", i); p->set(n, att);
        }
    }
    else p->set("numberOfLights", 0);
}

void Scene::generateInstances(vector<vec4>& out, int n, float ds, float da, float smin, float smax, const vector<vec4>* av)
{
    for(int i = 0; i < n; i++)
    {
        for(int t = 0; t < 80; t++)
        {
            float x = (rand() % 200) / 100.0f - 1.0f;
            float y = (rand() % 200) / 100.0f - 1.0f;
            bool ok = true;

            if(ds > 0.0f)
            {
                int k = 0;
                for(; k < out.size(); k++)
                {
                    float dx = x - out[k].x, dy = y - out[k].y;
                    if(dx * dx + dy * dy < ds * ds) break;
                }
                if(k < out.size()) ok = false;
            }

            if(ok && av && da > 0.0f)
            {
                int j = 0;
                for(; j < av->size(); j++)
                {
                    float dx = x - (*av)[j].x, dy = y - (*av)[j].y;
                    if(dx * dx + dy * dy < da * da) break;
                }
                if(j < av->size()) ok = false;
            }

            if(ok)
            {
                out.push_back(vec4(x, y, radians((float)(rand() % 360)), smin + ((rand() % 1000) / 1000.0f) * (smax - smin)));
                break;
            }
        }
    }
}

void Scene::updateFireflies()
{
    if(!fireflyReady)
    {
        for(int i = 0; i < 5; i++)
        {
            fireflyPos[i] = vec3(((rand() % 200) / 100.0f - 1.0f) * 1.6f, 0.40f + ((rand() % 100) / 100.0f) * 0.80f, ((rand() % 200) / 100.0f - 1.0f) * 1.6f);
            vec3 v(((rand() % 200) - 100) / 80.0f, ((rand() % 200) - 100) / 160.0f, ((rand() % 200) - 100) / 80.0f);

            float L = length(v);
            fireflyVel[i] = (L > 0.0f ? v / L : vec3(1, 0, 0)) * 1.0f;
        }
        lastFireflyTime = (float)glfwGetTime();
        fireflyReady = true;
    }

    float now = (float)glfwGetTime();
    float dt = clamp(now - lastFireflyTime, 0.0f, 0.05f);
    lastFireflyTime = now;

    const float mn[3] = {-1.6f, 0.35f, -1.6f}, mx[3] = {1.6f, 1.20f, 1.6f}, repelR = 0.8f, repelK = 1.8f;
    for(int i = 0; i < 5; i++)
    {
        fireflyVel[i] += vec3(((rand() % 200) - 100) / 400.0f, ((rand() % 200) - 100) / 800.0f, ((rand() % 200) - 100) / 400.0f);

        vec3 F(0.0f);
        for(int j = 0; j < 5; j++)
        {
            if(j != i)
            {
                vec3 d = fireflyPos[i] - fireflyPos[j];
                float L = length(d);
                if(L > 0.0001f && L < repelR) F += (d / L) * ((repelR - L) * repelK);
            }
        }
        fireflyVel[i] += F * dt;

        float v = length(fireflyVel[i]);
        if(v > 0.0f) fireflyVel[i] *= clamp(v, 0.6f, 2.5f) / v;

        fireflyPos[i] += fireflyVel[i] * dt;
        for(int k = 0; k < 3; k++)
        {
            if(fireflyPos[i][k] < mn[k])
            {
                fireflyPos[i][k] = mn[k]; 
                if(fireflyVel[i][k] < 0) fireflyVel[i][k] = -fireflyVel[i][k];
            }
            if(fireflyPos[i][k] > mx[k])
            {
                fireflyPos[i][k] = mx[k];
                if(fireflyVel[i][k] > 0) fireflyVel[i][k] = -fireflyVel[i][k];
            }
        }
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

    constantShaderProgram = new ShaderProgram("constant.vert", "constant.frag");
    lambertShaderProgram = new ShaderProgram("lambert.vert", "lambert.frag");
    phongShaderProgram = new ShaderProgram("phong.vert", "phong.frag");
    blinnShaderProgram = new ShaderProgram("blinn.vert", "blinn.frag");

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

    if(!pointLight) pointLight = new Light(vec3(0.0f, 0.0f, 0.0f));
}

void Scene::setupScene3()
{
    plainModel = new Model();
    plainModel->setupModel(plain, sizeof(plain), true);
    plainShaderProgram = new ShaderProgram("forest.vert", "plain.frag");
    plainObject = new DrawableObject(plainModel, plainShaderProgram, 6, GL_TRIANGLES);

	models.push_back(plainModel); shaderPrograms.push_back(plainShaderProgram); objects.push_back(plainObject);

    treeModel = new Model();
    treeModel->setupModel(tree, sizeof(tree), true);
    treeShaderProgram = new ShaderProgram("forest.vert", "tree.frag");
    treeObject = new DrawableObject(treeModel, treeShaderProgram, 92814, GL_TRIANGLES);

	models.push_back(treeModel); shaderPrograms.push_back(treeShaderProgram); objects.push_back(treeObject);

    bushModel = new Model();
    bushModel->setupModel(bushes, sizeof(bushes), true);
    bushShaderProgram = new ShaderProgram("forest.vert", "bush.frag");
    bushObject = new DrawableObject(bushModel, bushShaderProgram, 8730, GL_TRIANGLES);

	models.push_back(bushModel); shaderPrograms.push_back(bushShaderProgram); objects.push_back(bushObject);

    fireflyShaderProgram = new ShaderProgram("forest.vert", "firefly.frag");
    fireflyObject = new DrawableObject(sphereModel, fireflyShaderProgram, 2880, GL_TRIANGLES);

    shaderPrograms.push_back(fireflyShaderProgram); objects.push_back(fireflyObject);

	camera2->addObserver(plainShaderProgram);
	camera2->addObserver(treeShaderProgram);
	camera2->addObserver(bushShaderProgram);
    camera2->addObserver(fireflyShaderProgram);

    srand(time(NULL));
    treeInstances.clear(); bushInstances.clear();
    generateInstances(treeInstances, 100, 0.10f, 0.00f, 0.07f, 0.10f, nullptr);
    generateInstances(bushInstances, 70, 0.00f, 0.05f, 0.18f, 0.22f, &treeInstances);
}

void Scene::setupScene4()
{
    solarShaderProgram = new ShaderProgram("solar.vert", "solar.frag");
    shaderPrograms.push_back(solarShaderProgram);

    sunObject = new DrawableObject(sphereModel, solarShaderProgram, 2880, GL_TRIANGLES);
    earthObject = new DrawableObject(sphereModel, solarShaderProgram, 2880, GL_TRIANGLES);
    moonObject = new DrawableObject(sphereModel, solarShaderProgram, 2880, GL_TRIANGLES);

    objects.push_back(sunObject); objects.push_back(earthObject); objects.push_back(moonObject);
    camera2->addObserver(solarShaderProgram);

    if(!pointLight) pointLight = new Light(vec3(0.0f, 0.0f, 0.0f));
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
        applyLight(constantShaderProgram);
        applyLight(lambertShaderProgram);
        applyLight(phongShaderProgram);
        applyLight(blinnShaderProgram);

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
        applyLight(plainShaderProgram);
        applyLight(treeShaderProgram);
        applyLight(bushShaderProgram);

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

        for(int i = 0; i < 5; i++)
        {
            mat4 M(1.0f);
            TransformationComposite TC;
            TC.addComponent(new Translation(fireflyPos[i]));
            TC.addComponent(new Scale(vec3(0.04f)));
            TC.apply(M);

            fireflyObject->draw(M);
        }
    }
    else if(currentScene == 4)
    {
        pointLight->setPosition(vec3(0.0f));
		applyLight(solarShaderProgram);

        vec3 earthPos = vec3(2.0f * cos(0.50f * (float)glfwGetTime()), 2.0f * sin(0.50f * (float)glfwGetTime()), 0.0f);
        vec3 moonPos = vec3(0.60f * cos(1.50f * (float)glfwGetTime()), 0.60f * sin(1.50f * (float)glfwGetTime()), 0.0f);

        vec3 colors[3] = {vec3(1.0f, 0.93f, 0.30f), vec3(0.20f, 0.45f, 0.90f), vec3(0.65f, 0.65f, 0.65f)};
        vec3 scales[3] = {vec3(1.0f), vec3(0.35), vec3(0.12f)};
        vec3 positions[3] = {vec3(0.0f), earthPos, earthPos + moonPos};

        float emissive[3] = {0.8f, 0.0f, 0.0f};

        DrawableObject* objects[3] = {sunObject, earthObject, moonObject};
        solarShaderProgram->use();

        for(int i = 0; i < 3; ++i)
        {
            mat4 M(1.0f);
            TransformationComposite TC;
            TC.addComponent(new Translation(positions[i]));
            TC.addComponent(new Scale(scales[i]));
            TC.apply(M);

            solarShaderProgram->set("objectColor", colors[i]);
            solarShaderProgram->set("emissive", emissive[i]);
            objects[i]->draw(M);
        }
    }
}