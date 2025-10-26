#include "Camera.h"

Camera::Camera(vec3 startPos, float startYaw, float startPitch, float aspectRatio) : position(startPos), worldUp(vec3(0.0f, 1.0f, 0.0f)), yaw(startYaw), pitch(startPitch), speed(2.5f), sensitivity(0.1f), firstMouse(true), aspect(aspectRatio)
{
    updateCameraVectors();
}

mat4 Camera::getViewMatrix() const
{
    return lookAt(position, position + front, up);
}

mat4 Camera::getProjectionMatrix() const
{
    return perspective(radians(fov), aspect, 0.1f, 100.0f);
}

void Camera::processKeyboard(GLFWwindow* window, float deltaTime)
{
    float velocity = speed * deltaTime;
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) position += front * velocity;
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) position -= front * velocity;
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) position -= right * velocity;
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) position += right * velocity;

    fprintf(stderr, "[Camera] Keyboard -> pos=(%.2f, %.2f, %.2f), yaw=%.2f, pitch=%.2f\n", position.x, position.y, position.z, yaw, pitch);
    fprintf(stderr, "[Camera] notify(view, proj)\n");
    notify(getViewMatrix(), getProjectionMatrix());
}

void Camera::processMouse(double xpos, double ypos, bool rightMousePressed)
{
    if(!rightMousePressed)
    {
        firstMouse = true;
        return;
    }
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

	float xoffset = (float)(xpos - lastX), yoffset = (float)(lastY - ypos);
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)  pitch = 89.0f;
    if(pitch < -89.0f) pitch = -89.0f;
    updateCameraVectors();

    fprintf(stderr, "[Camera] Mouse -> yaw=%.2f, pitch=%.2f (xoff=%.2f, yoff=%.2f)\n", yaw, pitch, xoffset, yoffset);
    fprintf(stderr, "[Camera] notify(view, proj)\n");
    notify(getViewMatrix(), getProjectionMatrix());
}

void Camera::setPosition(const vec3& newPos)
{
    position = newPos;
}

void Camera::setRotation(float newYaw, float newPitch)
{
    yaw = newYaw;
    pitch = newPitch;
    updateCameraVectors();
}

void Camera::setAspect(float a)
{
    if(a <= 0.0f) return;
    aspect = a;
    fprintf(stderr, "[Camera] Aspect=%.3f -> notify(view, proj)\n", aspect);
    notify(getViewMatrix(), getProjectionMatrix());
}

void Camera::setFov(float f)
{
    if(f < 1.0f) f = 1.0f;
    if(f > 170.0f) f = 170.0f;
    fov = f;
    fprintf(stderr, "[Camera] FOV=%.1f -> notify(view, proj)\n", fov);
    notify(getViewMatrix(), getProjectionMatrix());
}

vec3 Camera::getPosition() const
{
    return position;
}

void Camera::reset(vec3 pos, float yawAngle, float pitchAngle)
{
    position = pos;
    yaw = yawAngle;
    pitch = pitchAngle;
    updateCameraVectors();
    fprintf(stderr, "[Camera] Reset -> notify(view, proj)\n");
    notify(getViewMatrix(), getProjectionMatrix());
}

void Camera::notifyMatrices() 
{
    notify(getViewMatrix(), getProjectionMatrix());
}

void Camera::updateCameraVectors()
{
    vec3 f;
    f.x = cos(radians(yaw)) * cos(radians(pitch));
    f.y = sin(radians(pitch));
    f.z = sin(radians(yaw)) * cos(radians(pitch));
    front = normalize(f);

    right = normalize(cross(front, worldUp));
    up = normalize(cross(right, front));
}