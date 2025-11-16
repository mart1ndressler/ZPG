#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include "Scene.h"

class Application
{
    public:
        Application();
        ~Application();
        void run();

    private:
        void initWindow();
        void initOpenGL();
        void handleInput();

        GLFWwindow* window = nullptr;
        Scene* scene = nullptr;

		bool mWasDown = false, tWasDown = false, vWasDown = false, lWasDown = false, fWasDown = false, rWasDown = false, leftWasDown = false;
        int fovIdx = 0;
};