#pragma once
#include <glm/glm.hpp>

using namespace glm;

class TransformationComponent
{
    public:
        virtual void apply(mat4& matrix) const = 0;
        virtual ~TransformationComponent() = default;
};