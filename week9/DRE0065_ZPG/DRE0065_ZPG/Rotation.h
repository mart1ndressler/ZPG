#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "TransformationComponent.h"

using namespace glm;

class Rotation : public TransformationComponent
{
    public:
        Rotation(float angle, const vec3& axis);
        void apply(mat4& matrix) const override;

    private:
        float angle;
        vec3 axis;
};