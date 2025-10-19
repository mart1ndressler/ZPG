#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "DrawableObject.h"
#include "TransformationComposite.h"
#include "Rotation.h"
#include "Translation.h"
#include "Scale.h"
#include "Camera.h"
#include "Light.h"

using namespace glm;
using namespace std;

class Scene
{
    public:
        Scene();
        ~Scene();
        void draw();
        void setScene(int id);
        void updateCamera(GLFWwindow* window, float deltaTime, double xpos, double ypos, bool rightPressed);
        
    private:
        Model* triangleModel = nullptr;
        Model* sphereModel = nullptr;
        Model* plainModel = nullptr;
        Model* treeModel = nullptr;
        Model* bushModel = nullptr;

        ShaderProgram* triangleShaderProgram = nullptr;
        ShaderProgram* constantShaderProgram = nullptr;
        ShaderProgram* lambertShaderProgram = nullptr;
        ShaderProgram* phongShaderProgram = nullptr;
        ShaderProgram* blinnShaderProgram = nullptr;
        ShaderProgram* plainShaderProgram = nullptr;
        ShaderProgram* treeShaderProgram = nullptr;
        ShaderProgram* bushShaderProgram = nullptr;
        ShaderProgram* solarShaderProgram = nullptr;

        DrawableObject* triangleObject = nullptr;
        DrawableObject* constantSphereObject = nullptr;
        DrawableObject* lambertSphereObject = nullptr;
        DrawableObject* phongSphereObject = nullptr;
        DrawableObject* blinnSphereObject = nullptr;
        DrawableObject* plainObject = nullptr;
        DrawableObject* treeObject = nullptr;
        DrawableObject* bushObject = nullptr;
        DrawableObject* sunObject = nullptr;
        DrawableObject* earthObject = nullptr;
        DrawableObject* moonObject = nullptr;

        vector<vec4> treeInstances;
        vector<vec4> bushInstances;

        vector<Model*> models;
        vector<ShaderProgram*> shaderPrograms;
        vector<DrawableObject*> objects;

        Camera* camera1 = nullptr;
        Camera* camera2 = nullptr;
        Camera* activeCamera = nullptr;

        Light* pointLight = nullptr;

        int currentScene = 1;

        void setupScene1();
        void setupScene2();
        void setupScene3();
        void setupScene4();
};
#endif