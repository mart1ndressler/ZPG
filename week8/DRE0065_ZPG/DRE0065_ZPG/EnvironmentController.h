#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <cstdlib>
#include <algorithm>

using namespace std;
using namespace glm;

class EnvironmentController
{
    public:
        EnvironmentController();
        void generateInstances(vector<vec4>& out, int count, float minDistBetween, float minDistFromAvoid, float scaleMin, float scaleMax, const vector<vec4>* avoidList);
        void updateFireflies();

        vec3 fireflyPositions[16] = {}, fireflyVelocities[16] = {};
        int fireflyTotal = 8;

    private:
        bool firefliesReady = false;
        float lastFireflyTime = 0.0f;
};