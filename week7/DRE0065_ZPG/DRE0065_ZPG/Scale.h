#pragma once
#ifndef SCALE_H
#define SCALE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "TransformationComponent.h"

using namespace glm;

class Scale : public TransformationComponent
{
    public:
        Scale(const vec3& scale);
        void apply(mat4& matrix) const override;

    private:
        vec3 scale;
};
#endif