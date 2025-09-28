#include "Application.h"

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
    printf("key_callback [%d,%d,%d,%d] \n", key, scancode, action, mods);
}
static void window_focus_callback(GLFWwindow* window, int focused)
{
    printf("window_focus_callback \n");
}
static void window_iconify_callback(GLFWwindow* window, int iconified)
{
    printf("window_iconify_callback \n");
}
static void window_size_callback(GLFWwindow* window, int width, int height)
{
    printf("resize %d, %d \n", width, height);
    glViewport(0, 0, width, height);
}
static void cursor_callback(GLFWwindow* window, double x, double y) 
{
    printf("cursor_callback \n");
}
static void button_callback(GLFWwindow* window, int button, int action, int mode) 
{
    if(action == GLFW_PRESS) printf("button_callback [%d,%d,%d]\n", button, action, mode);
}

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
    glfwSetErrorCallback(error_callback);
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

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetMouseButtonCallback(window, button_callback);
    glfwSetWindowFocusCallback(window, window_focus_callback);
    glfwSetWindowIconifyCallback(window, window_iconify_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float ratio = width / (float)height;
    glViewport(0, 0, width, height);
}

void Application::initOpenGL()
{
    glewExperimental = GL_TRUE;
    glewInit();

    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
    printf("Vendor %s\n", glGetString(GL_VENDOR));
    printf("Renderer %s\n", glGetString(GL_RENDERER));
    printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    printf("Using GLFW %i.%i.%i\n", major, minor, revision);
}

void Application::run()
{
    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene->draw();
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}