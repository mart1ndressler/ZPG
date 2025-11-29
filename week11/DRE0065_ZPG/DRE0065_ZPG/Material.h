#pragma once
#include <glm/glm.hpp>
#include "ShaderProgram.h"

using namespace glm;

class Material
{
    public:
        Material(float ra = 1.0f, float rd = 1.0f, float rs = 0.0f, float h = 16.0f);
        Material(const vec3& baseColor, float ra, float rd, float rs = 0.0f, float h = 16.0f);
        void applyM(ShaderProgram* shaderProgram) const;

    private:
        float m_ra, m_rd, m_rs, m_h;
        vec3 m_base;
        bool m_hasBase;
};