#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>
#include <stdio.h>

static int g_sign = 1;
static int axis = 2;

static void error_callback(int error, const char* description) {fputs(description, stderr);}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
	if(key == GLFW_KEY_SPACE && action == GLFW_PRESS) g_sign = -g_sign;
	if(key == GLFW_KEY_X && action == GLFW_PRESS) axis = 0;
	if(key == GLFW_KEY_Y && action == GLFW_PRESS) axis = 1;
	if(key == GLFW_KEY_Z && action == GLFW_PRESS) axis = 2;
	printf("key_callback [%d,%d,%d,%d] \n", key, scancode, action, mods);
}

static void window_focus_callback(GLFWwindow* window, int focused) {printf("window_focus_callback \n");}
static void window_iconify_callback(GLFWwindow* window, int iconified) {printf("window_iconify_callback \n");}

static void window_size_callback(GLFWwindow* window, int width, int height)
{
	printf("resize %d, %d \n", width, height);
	glViewport(0, 0, width, height);
}

static void cursor_callback(GLFWwindow* window, double x, double y) {printf("cursor_callback \n");}
static void button_callback(GLFWwindow* window, int button, int action, int mode) {if(action == GLFW_PRESS) printf("button_callback [%d,%d,%d]\n", button, action, mode);}

glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.01f, 100.0f);
glm::mat4 View = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
glm::mat4 Model = glm::mat4(1.0f);

int main(void)
{
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);

	if(!glfwInit()) exit(EXIT_FAILURE);
	window = glfwCreateWindow(1280, 920, "ZPG", NULL, NULL);
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

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);

	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glTranslatef(-0.2f, 0.2f, 0.f);
		glRotatef((float)glfwGetTime() * 50.f * g_sign, axis == 0, axis == 1, axis == 2);
		glTranslatef(0.2f, -0.2f, 0.f);

		glBegin(GL_QUADS);
			glColor3f(1.f, 0.f, 0.f);
			glVertex3f(-0.2f, -0.2f, 0.f);
			glColor3f(0.f, 1.f, 0.f);
			glVertex3f(0.2f, -0.2f, 0.f);
			glColor3f(0.f, 0.f, 1.f);
			glVertex3f(0.2f, 0.2f, 0.f);
			glColor3f(1.f, 1.f, 0.f);
			glVertex3f(-0.2f, 0.2f, 0.f);
		glEnd();
		glfwSwapBuffers(window);

		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}