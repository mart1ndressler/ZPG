#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

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
        bool mWasDown = false, tWasDown = false, vWasDown = false, lWasDown = false, fWasDown = false;
        int fovIdx = 1;
};
#endif