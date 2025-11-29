#include "EnvironmentController.h"

EnvironmentController::EnvironmentController() {}

void EnvironmentController::generateInstances(vector<vec4>& out, int count, float minDistBetween, float minDistFromAvoid, float scaleMin, float scaleMax, const vector<vec4>* avoidList)
{
    for(int i = 0; i < count; i++)
    {
        for(int attempt = 0; attempt < 100; attempt++)
        {
            float gx = (rand() % 200) / 100.0f - 1.0f;
            float gy = (rand() % 200) / 100.0f - 1.0f;
            bool ok = true;

            if(minDistBetween > 0.0f)
            {
                int k = 0;
                for(; k < (int)out.size(); k++)
                {
                    float dx = gx - out[k].x, dy = gy - out[k].y;
                    if(dx * dx + dy * dy < minDistBetween * minDistBetween) break;
                }
                if(k < (int)out.size()) ok = false;
            }

            if(ok && avoidList && minDistFromAvoid > 0.0f)
            {
                int j = 0;
                for(; j < (int)avoidList->size(); j++)
                {
                    float dx = gx - (*avoidList)[j].x, dy = gy - (*avoidList)[j].y;
                    if(dx * dx + dy * dy < minDistFromAvoid * minDistFromAvoid) break;
                }
                if(j < (int)avoidList->size()) ok = false;
            }

            if(ok)
            {
                float angleRad = radians((float)(rand() % 360));
                float scaleVal = scaleMin + ((rand() % 1000) / 1000.0f) * (scaleMax - scaleMin);
                out.push_back(vec4(gx, gy, angleRad, scaleVal));
                break;
            }
        }
    }
}

void EnvironmentController::updateFireflies()
{
    if(!firefliesReady)
    {
        for(int i = 0; i < fireflyTotal; i++)
        {
            fireflyPositions[i] = vec3(((rand() % 200) / 100.0f - 1.0f) * 1.6f, 0.40f + ((rand() % 100) / 100.0f) * 0.80f, ((rand() % 200) / 100.0f - 1.0f) * 1.6f);
            vec3 v(((rand() % 200) - 100) / 80.0f, ((rand() % 200) - 100) / 160.0f, ((rand() % 200) - 100) / 80.0f);
            float L = length(v);
            fireflyVelocities[i] = (L > 0.0f ? v / L : vec3(1, 0, 0)) * 1.0f;
        }
        lastFireflyTime = (float)glfwGetTime();
        firefliesReady = true;
    }

    float now = (float)glfwGetTime();
    float dt = clamp(now - lastFireflyTime, 0.0f, 0.05f);
    lastFireflyTime = now;

    vec3 minB(-3.6f, 0.35f, -3.6f), maxB(3.6f, 0.80f, 3.6f);
    for(int i = 0; i < fireflyTotal; i++)
    {
        fireflyVelocities[i] += vec3(((rand() % 200) - 100) / 400.0f, ((rand() % 200) - 100) / 800.0f, ((rand() % 200) - 100) / 400.0f);

        float vLen = length(fireflyVelocities[i]);
        if(vLen > 0.0f) fireflyVelocities[i] *= clamp(vLen, 0.6f, 2.5f) / vLen;

        fireflyPositions[i] += fireflyVelocities[i] * dt;
        for(int k = 0; k < 3; k++)
        {
            if(fireflyPositions[i][k] < minB[k])
            {
                fireflyPositions[i][k] = minB[k];
                if(fireflyVelocities[i][k] < 0) fireflyVelocities[i][k] = -fireflyVelocities[i][k];
            }
            if(fireflyPositions[i][k] > maxB[k])
            {
                fireflyPositions[i][k] = maxB[k];
                if(fireflyVelocities[i][k] > 0) fireflyVelocities[i][k] = -fireflyVelocities[i][k];
            }
        }
    }
}

void EnvironmentController::plantInstanceAt(const vec3& worldPos, float forestExtent, vector<vec4>& treeInstances)
{
    float nx = worldPos.x / forestExtent, ny = -worldPos.z / forestExtent;
    float angle = radians((float)(rand() % 360)), scaleMin = 0.07f, scaleMax = 0.10f;
    float scale = scaleMin + ((rand() % 100) / 100.0f) * (scaleMax - scaleMin);

    treeInstances.push_back(vec4(nx, ny, angle, scale));
    fprintf(stderr, "[PLANT] New tree = [%f, %f, %f, %f]\n", nx, ny, angle, scale);
}

void EnvironmentController::initForest(float houseAvoidRadius, vector<vec4>& treeInstances, vector<vec4>& bushInstances)
{
    vector<vec4> avoidModels = {vec4(0.0f, 0.0f, 0.0f, 0.0f)};
    float houseAvoid_T_B = houseAvoidRadius;

    generateInstances(treeInstances, 500, 0.10f, houseAvoid_T_B, 0.07f, 0.10f, &avoidModels);
    generateInstances(bushInstances, 250, 0.03f, houseAvoid_T_B, 0.18f, 0.22f, &avoidModels);
}

void EnvironmentController::initArcadeGrid(vec3* positions, int rows, int cols, const vec3& start, float step)
{
    int idx = 0;
    for(int row = 0; row < rows; row++)
    {
        for(int col = 0; col < cols; col++) positions[idx++] = vec3(start.x + col * step, start.y, start.z + row * step);
    }
}

void EnvironmentController::initArcadeShreks(int* indices, float* lastTimes, int count, int cellCount, float baseTime)
{
    for(int i = 0; i < count; i++)
    {
        while(true)
        {
            int k;
            indices[i] = rand() % cellCount;
            for(k = 0; k < i; k++)
            {
                if(indices[k] == indices[i]) break;
            }
            if(k == i) break;
        }
        lastTimes[i] = baseTime + i * 1.0f;
    }
}

void EnvironmentController::updateArcadeShreks(float t, int* indices, float* lastTimes, int count, int cellCount, float intervalSec)
{
    for(int i = 0; i < count; i++)
    {
        if(t - lastTimes[i] >= intervalSec)
        {
            lastTimes[i] += intervalSec;
            int oldIndex = indices[i], newIndex = oldIndex;
            while(true)
            {
                int k;
                newIndex = rand() % cellCount;
                if(newIndex == oldIndex) continue;

                for(k = 0; k < count; ++k)
                {
                    if(k != i && indices[k] == newIndex) break;
                }
                if(k == count) break;
            }
            indices[i] = newIndex;
        }
    }
}