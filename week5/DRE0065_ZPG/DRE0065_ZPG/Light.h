#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <vector>
#include <algorithm>

#include "ShaderProgram.h"

using namespace glm;
using namespace std;

class Light
{
    public:
        Light(const vec3& position);
        vec3 getPosition() const;
        void setPosition(const vec3& p);
        void attach(ShaderProgram* program);
        void detach(ShaderProgram* program);
        void pushToPrograms() const;

    private:
        vec3 position;
        vector<ShaderProgram*> programs;
        void pushTo(ShaderProgram* program) const;
};
#endif