#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <ctime>
#include <cmath>
#include "DrawableObject.h"
#include "TransformationComposite.h"
#include "Rotation.h"
#include "Translation.h"
#include "Scale.h"
#include "Camera.h"
#include "Light.h"
#include "LightingController.h"
#include "EnvironmentController.h"
#include "Material.h"
#include "Texture.h"
#include "Skybox.h"

using namespace glm;
using namespace std;

class Scene
{
    public:
        Scene();
        ~Scene();
        void draw();
        void setScene(int id);
        int getScene();
        void updateCamera(GLFWwindow* window, float deltaTime, double xpos, double ypos, bool rightPressed);
        void setAspect(float a);
        void setFov(float f);
        void handleClick(int mouseX, int mouseY, int width, int height);
        void setTransformMode(int mode);
        void evalBezierSegment(const vec3& p0, const vec3& p1, const vec3& p2, const vec3& p3, float t, vec3& outPos, vec3& outTangent);

		int scene2Mode = 0, scene2Lights = 0, forestLightMode = 0, transformMode = 0;
        bool flashlightOn = false, skyboxOn = true;
        
    private:
        void setupScene1();
        void setupScene2();
        void setupScene3();
        void setupScene4();
		void setupScene5();
		void setupScene6();

        Model* triangleModel = nullptr;
        Model* sphereModel = nullptr;
        Model* plainModel = nullptr;
        Model* treeModel = nullptr;
        Model* bushModel = nullptr;
        Model* houseModel = nullptr;
        Model* formulaModel = nullptr;
        Model* cubeModel = nullptr;
        Model* shrekModel = nullptr;
        Model* toiletModel = nullptr;
        Model* fionaModel = nullptr;
        Model* planetModel = nullptr;
        Model* arcadeloginModel = nullptr;

        ShaderProgram* triangleShaderProgram = nullptr;
        ShaderProgram* constantShaderProgram = nullptr;
        ShaderProgram* lambertShaderProgram = nullptr;
        ShaderProgram* phongShaderProgram = nullptr;
        ShaderProgram* blinnShaderProgram = nullptr;
		ShaderProgram* plainShaderProgram = nullptr;
		ShaderProgram* treeShaderProgram = nullptr;
		ShaderProgram* bushShaderProgram = nullptr;
        ShaderProgram* forestShaderProgram = nullptr;
        ShaderProgram* fireflyShaderProgram = nullptr;
        ShaderProgram* solarShaderProgram = nullptr;
        ShaderProgram* arcadeShaderProgram = nullptr;
        ShaderProgram* bezierShaderProgram = nullptr;

        DrawableObject* triangleObject = nullptr;
        DrawableObject* constantSphereObject = nullptr;
        DrawableObject* lambertSphereObject = nullptr;
        DrawableObject* phongSphereObject = nullptr;
        DrawableObject* blinnSphereObject = nullptr;
        DrawableObject* plainObject = nullptr;
        DrawableObject* treeObject = nullptr;
        DrawableObject* bushObject = nullptr;
        DrawableObject* houseObject = nullptr;
        DrawableObject* formulaObject = nullptr;
        DrawableObject* cubeObject = nullptr;
        DrawableObject* shrekObject = nullptr;
        DrawableObject* toiletObject = nullptr;
        DrawableObject* fionaObject = nullptr;
        DrawableObject* fireflyObject = nullptr;
        DrawableObject* sunObject = nullptr;
        DrawableObject* earthObject = nullptr;
        DrawableObject* moonObject = nullptr;
		DrawableObject* mercuryObject = nullptr;
		DrawableObject* venusObject = nullptr;
		DrawableObject* marsObject = nullptr;
		DrawableObject* jupiterObject = nullptr;
		DrawableObject* saturnObject = nullptr;
		DrawableObject* uranusObject = nullptr;
		DrawableObject* neptuneObject = nullptr;
        DrawableObject* arcadePlainObject = nullptr;
        DrawableObject* arcadeloginObject = nullptr;
        DrawableObject* arcadeShrekObject = nullptr;
        DrawableObject* bezierPlainObject = nullptr;
        DrawableObject* bezierFormulaObject = nullptr;
        
        vector<vec4> treeInstances;
        vector<vec4> bushInstances;
        vector<vec3> bezierControlPoints;

        vector<Model*> models;
        vector<ShaderProgram*> shaderPrograms;
        vector<DrawableObject*> objects;
		vector<Texture*> textures;

        Camera* camera1 = nullptr;
        Camera* camera2 = nullptr;
        Camera* activeCamera = nullptr;

        Light* pointLight = nullptr;
        LightingController* lighting = nullptr;
        EnvironmentController* environment = nullptr;

        Texture* grassTexture = nullptr;
        Texture* houseTexture = nullptr;
        Texture* formulaTexture = nullptr;
		Texture* cubeTexture = nullptr;
		Texture* shrekTexture = nullptr;
		Texture* toiletTexture = nullptr;
		Texture* fionaTexture = nullptr;
		Texture* sunTexture = nullptr;
		Texture* earthTexture = nullptr;
		Texture* moonTexture = nullptr;
		Texture* mercuryTexture = nullptr;
		Texture* venusTexture = nullptr;
		Texture* marsTexture = nullptr;
		Texture* jupiterTexture = nullptr;
		Texture* saturnTexture = nullptr;
		Texture* uranusTexture = nullptr;
		Texture* neptuneTexture = nullptr;
        Texture* arcadePlainTexture = nullptr;
		Texture* arcadeloginTexture = nullptr;

        Skybox* skybox = nullptr;
		Skybox* solarSkybox = nullptr;

        int currentScene = 1, arcadeScore = 0, arcadeShrekPosIndex[3] = {0, 1, 2};
		float forestExtent = 3.6f, arcadeShrekLastTime[3] = {0.0f, 0.5f, 1.0f}, bezierSpeed = 0.3f;
        bool skyboxPrevOn = true;

        vec3 skycubePos = vec3(0.0f), arcadePositions[36];
        mat4 customMatrix = mat4(1.0f);
};