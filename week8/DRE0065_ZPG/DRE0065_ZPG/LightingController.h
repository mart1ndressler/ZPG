#pragma once
#include <glm/glm.hpp>
#include <algorithm>
#include <cstdio>
#include <cmath>
#include "ShaderProgram.h"
#include "Light.h"

using namespace glm;
using namespace std;

class LightingController
{
    public:
        LightingController();
        void setPointLight(Light* plight);
        void applyL(ShaderProgram* shaderProgram, int currentScene, int scene2LightsCount = 0, int forestMode = 0, const vec3* fireflyPos = nullptr, int fireflyCount = 0, vec3 camPos = vec3(0.0f), vec3 camDir = vec3(0.0f, 0.0f, -1.0f), bool flashlightOn = false, const vec3* objectColor = nullptr, const float* emissiveVal = nullptr);

        int fireflyCap = 8;

    private:
        Light* pointLight = nullptr;
        vec3 scene2Fixed[5];
};