#pragma once
#ifndef ROTATION_H
#define ROTATION_H

#include "TransformationComponent.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
#endif