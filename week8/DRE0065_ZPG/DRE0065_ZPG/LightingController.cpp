#include "LightingController.h"

LightingController::LightingController() : pointLight(nullptr)
{
    scene2Fixed[0] = vec3(0.00f, 0.00f, 0.0f);
    scene2Fixed[1] = vec3(0.40f, 0.40f, 0.0f);
    scene2Fixed[2] = vec3(-0.40f, 0.40f, 0.0f);
    scene2Fixed[3] = vec3(-0.40f, -0.40f, 0.0f);
    scene2Fixed[4] = vec3(0.40f, -0.40f, 0.0f);
}

void LightingController::setPointLight(Light* plight)
{
    pointLight = plight;
}

void LightingController::applyL(ShaderProgram* shaderProgram, int currentScene, int scene2LightsCount, int forestMode, const vec3* fireflyPos, int fireflyCount, vec3 camPos, vec3 camDir, bool flashlightOn, const vec3* objectColor, const float* emissiveVal)
{
    if(!shaderProgram) return;
    shaderProgram->use();

    int lights = 16;
    if(pointLight) shaderProgram->set("lightPosition", pointLight->getPosition());

    if(currentScene == 2)
    {
        scene2LightsCount = std::max(0, std::min(scene2LightsCount, 16));
        shaderProgram->set("numberOfLights", scene2LightsCount);

        for(int i = 0; i < 5; i++)
        {
            char n[48];
            snprintf(n, sizeof(n), "uLights[%d].position", i); shaderProgram->set(n, vec4(scene2Fixed[i], 1.0f));
            snprintf(n, sizeof(n), "uLights[%d].diffuse", i); shaderProgram->set(n, vec4(1.0f));
            snprintf(n, sizeof(n), "uLights[%d].specular", i); shaderProgram->set(n, vec4(1.0f));
        }
        if(scene2LightsCount > 0) shaderProgram->set("lightPosition", scene2Fixed[0]);
    }
    else if(currentScene == 3)
    {
        shaderProgram->set("forestLightMode", forestMode);
        int usePoints = (forestMode == 1 || forestMode == 3) ? std::max(0, std::min(std::min(fireflyCount, fireflyCap), 16)) : 0;
        shaderProgram->set("numberOfLights", usePoints);

        for(int i = 0; i < lights; i++)
        {
            vec3 pos = (fireflyPos && i < usePoints) ? fireflyPos[i] : vec3(0.0f);
            char n[64];
            snprintf(n, sizeof(n), "uLights[%d].position", i); shaderProgram->set(n, vec4(pos, 1.0f));
            snprintf(n, sizeof(n), "uLights[%d].diffuse", i); shaderProgram->set(n, vec4(1.0f, 0.95f, 0.60f, 1.0f));
            snprintf(n, sizeof(n), "uLights[%d].specular", i); shaderProgram->set(n, vec4(0.0f));
            snprintf(n, sizeof(n), "uLights[%d].atten", i); shaderProgram->set(n, vec3(1.0f, 2.0f, 4.0f));
        }

        shaderProgram->set("dirLightDir", normalize(vec3(-0.6f, -0.7f, 0.2f)));
        shaderProgram->set("dirLightDiffuse", vec3(1.20f, 1.30f, 1.50f));
        shaderProgram->set("dirSplitOffset", -1.8f);
        shaderProgram->set("dirSplitSoftness", 4.0f);

        shaderProgram->set("flashlightOn", flashlightOn ? 1 : 0);
        shaderProgram->set("flashlightPos", camPos);
        shaderProgram->set("flashlightDir", normalize(camDir));
        shaderProgram->set("flashlightDiffuse", vec3(3.0f));
        shaderProgram->set("flashlightAtten", vec3(1.0f, 0.7f, 1.8f));
        shaderProgram->set("flashlightInnerCos", cos(radians(12.0f)));
        shaderProgram->set("flashlightOuterCos", cos(radians(16.0f)));
    }
    else if(currentScene == 4)
    {
        if(objectColor) shaderProgram->set("objectColor", *objectColor);
        if(emissiveVal) shaderProgram->set("emissive", *emissiveVal);
	}
    else shaderProgram->set("numberOfLights", 0);
}