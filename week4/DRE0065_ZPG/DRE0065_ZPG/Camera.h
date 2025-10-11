#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <cstdio>

#include "Subject.h"

using namespace glm;

class Camera : public Subject
{
    public:
        Camera(vec3 startPos, float startYaw, float startPitch, float aspectRatio);

        mat4 getViewMatrix() const;
        mat4 getProjectionMatrix() const;

        void processKeyboard(GLFWwindow* window, float deltaTime);
        void processMouse(double xpos, double ypos, bool rightMousePressed);

        void setPosition(const vec3& newPos) {position = newPos;}
        void setRotation(float newYaw, float newPitch) {yaw = newYaw; pitch = newPitch; updateCameraVectors();}
        vec3 getPosition() const {return position;}

        void reset(vec3 pos, float yaw, float pitch);
        void notifyMatrices() {notify(getViewMatrix(), getProjectionMatrix());}

    private:
        vec3 position, front, up, right, worldUp;
        float yaw, pitch, speed, sensitivity, aspect;
		bool firstMouse;
		double lastX, lastY;

        void updateCameraVectors();
};
#endif