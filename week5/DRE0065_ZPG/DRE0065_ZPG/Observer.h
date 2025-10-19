#pragma once
#ifndef OBSERVER_H
#define OBSERVER_H

#include <glm/glm.hpp>

using namespace glm;

class Observer
{
    public:
        virtual ~Observer() = default;
        virtual void updateViewProjection(const mat4& view, const mat4& projection) = 0;
};
#endif