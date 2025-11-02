#pragma once
#ifndef TRANSFORMATION_COMPONENT_H
#define TRANSFORMATION_COMPONENT_H

#include <glm/glm.hpp>

using namespace glm;

class TransformationComponent
{
    public:
        virtual void apply(mat4& matrix) const = 0;
        virtual ~TransformationComponent() = default;
};
#endif