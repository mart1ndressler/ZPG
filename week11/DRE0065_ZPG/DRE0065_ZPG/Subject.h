#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include <cstdio>
#include "Observer.h"

using namespace glm;
using namespace std;

class Subject
{
    public:
        void addObserver(Observer* obs)
        {
            if(!obs) return;
            if(find(observers.begin(), observers.end(), obs) == observers.end())
            {
                observers.push_back(obs);
                //fprintf(stderr, "[Observer] added -> total=%zu\n", observers.size());
            }
        }

        void removeObserver(Observer* obs)
        {
            auto it = find(observers.begin(), observers.end(), obs);
            if(it != observers.end())
            {
                observers.erase(it);
                //printf(stderr, "[Observer] removed -> total=%zu\n", observers.size());
            }
        }

        void notify(const mat4& view, const mat4& projection)
        {
            //fprintf(stderr, "[Observer] notify -> count=%zu\n", observers.size());
            for(auto* observer : observers) if(observer) observer->updateViewProjection(view, projection);
        }

        void notifyLight(const vec3& lightPos)
        {
            //fprintf(stderr, "[Subject] notifyLight: pos=(%f, %f, %f)\n", lightPos.x, lightPos.y, lightPos.z);
            for(auto* observer : observers) if(observer) observer->updateLight(lightPos);
        }

    private:
        vector<Observer*> observers;
};