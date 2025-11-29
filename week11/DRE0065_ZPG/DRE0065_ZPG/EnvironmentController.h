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
        void initForest(float houseAvoidRadius, vector<vec4>& treeInstances, vector<vec4>& bushInstances);
        void initArcadeGrid(vec3* positions, int rows, int cols, const vec3& start, float step);
        void initArcadeShreks(int* indices, float* lastTimes, int count, int cellCount, float baseTime);
        void updateArcadeShreks(float t, int* indices, float* lastTimes, int count, int cellCount, float intervalSec);

        vec3 fireflyPositions[16] = {}, fireflyVelocities[16] = {};
        int fireflyTotal = 8;

    private:
        bool firefliesReady = false;
        float lastFireflyTime = 0.0f;
};