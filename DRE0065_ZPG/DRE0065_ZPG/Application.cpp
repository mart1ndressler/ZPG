#include "Application.h"

Application::Application()
{
    initWindow();
    initOpenGL();
    scene = new Scene();
}

Application::~Application()
{
    delete scene;
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::initWindow()
{
    if(!glfwInit())
    {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(800, 600, "DRE0065 - ZPG", NULL, NULL);
    if(!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

void Application::initOpenGL()
{
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);

    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
    printf("Vendor %s\n", glGetString(GL_VENDOR));
    printf("Renderer %s\n", glGetString(GL_RENDERER));
    printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    printf("Using GLFW %i.%i.%i\n", major, minor, revision);
}

void Application::handleInput()
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) scene->setScene(1);
    if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) scene->setScene(2);
    if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) scene->setScene(3);
    if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) scene->setScene(4);
}

void Application::run()
{
    float lastFrame = 0.0f;
    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = (float)glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        handleInput();

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        bool rightPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

        scene->updateCamera(window, deltaTime, xpos, ypos, rightPressed);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene->draw();

        glfwSwapBuffers(window);
    }
}
