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
        GLFWwindow* window = nullptr;
        Scene* scene = nullptr;

        void initWindow();
        void initOpenGL();
};
#endif