#pragma once
#ifndef SCALE_H
#define SCALE_H

#include "TransformationComponent.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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