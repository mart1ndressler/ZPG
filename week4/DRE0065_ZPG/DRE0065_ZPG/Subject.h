#pragma once
#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>
#include <algorithm>
#include <cstdio>
#include <glm/glm.hpp>

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
                fprintf(stderr, "[Observer] added -> total=%zu\n", observers.size());
            }
        }

        void removeObserver(Observer* obs)
        {
            auto it = find(observers.begin(), observers.end(), obs);
            if(it != observers.end())
            {
                observers.erase(it);
                fprintf(stderr, "[Observer] removed -> total=%zu\n", observers.size());
            }
        }

        void notify(const mat4& view, const mat4& projection)
        {
            fprintf(stderr, "[Observer] notify -> count=%zu\n", observers.size());
            for(auto* o : observers) if(o) o->updateViewProjection(view, projection);
        }

    private:
        vector<Observer*> observers;
};
#endif