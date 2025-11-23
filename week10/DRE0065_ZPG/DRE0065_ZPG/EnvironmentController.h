#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <cstdlib>

using namespace std;
using namespace glm;

class EnvironmentController
{
    public:
        EnvironmentController();
        void generateInstances(vector<vec4>& out, int count, float minDistBetween, float minDistFromAvoid, float scaleMin, float scaleMax, const vector<vec4>* avoidList);
        void updateFireflies();
        void plantInstanceAt(const vec3& worldPos, float forestExtent, vector<vec4>& treeInstances);

        vec3 fireflyPositions[16] = {}, fireflyVelocities[16] = {};
        int fireflyTotal = 8;

    private:
        bool firefliesReady = false;
        float lastFireflyTime = 0.0f;
};