#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <ctime>
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

        int scene2Mode = 0, scene2Lights = 0, forestLightMode = 0;
        bool flashlightOn = false, skyboxOn = true;
        
    private:
        void setupScene1();
        void setupScene2();
        void setupScene3();
        void setupScene4();

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

        ShaderProgram* triangleShaderProgram = nullptr;
        ShaderProgram* constantShaderProgram = nullptr;
        ShaderProgram* lambertShaderProgram = nullptr;
        ShaderProgram* phongShaderProgram = nullptr;
        ShaderProgram* blinnShaderProgram = nullptr;
        ShaderProgram* plainShaderProgram = nullptr;
        ShaderProgram* treeShaderProgram = nullptr;
        ShaderProgram* bushShaderProgram = nullptr;
		ShaderProgram* modelsShaderProgram = nullptr;
        ShaderProgram* fireflyShaderProgram = nullptr;
		ShaderProgram* skyboxShaderProgram = nullptr;
        ShaderProgram* solarShaderProgram = nullptr;

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
        
        vector<vec4> treeInstances;
        vector<vec4> bushInstances;

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

        Skybox* skybox = nullptr;

        int currentScene = 1;
		float forestExtent = 3.6f;
        bool skyboxPrevOn = true;
        vec3 skycubePos = vec3(0.0f);
};