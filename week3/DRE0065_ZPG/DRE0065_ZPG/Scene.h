#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <random>

#include "DrawableObject.h"
#include "TransformationComposite.h"
#include "Rotation.h"
#include "Translation.h"
#include "Scale.h"

using namespace glm;
using namespace std;

class Scene
{
    public:
        Scene();
        ~Scene();
        void draw();
        void setScene(int id);
        
    private:
        Model* triangleModel = nullptr;
        ShaderProgram* triangleShaderProgram = nullptr;
        DrawableObject* triangleObject = nullptr;

        Model* sphereModel = nullptr;
        ShaderProgram* sphereShaderProgram = nullptr;
        DrawableObject* sphereObject = nullptr;

        Model* treeModel = nullptr;
        ShaderProgram* treeShaderProgram = nullptr;
        DrawableObject* treeObject = nullptr;

        Model* bushModel = nullptr;
        ShaderProgram* bushShaderProgram = nullptr;
        DrawableObject* bushObject = nullptr;

        vector<vec4> treeInstances;
        vector<vec4> bushInstances;

        int currentScene = 1;

        void setupScene1();
        void setupScene2();
        void setupScene3();
};
#endif