#include "Material.h"

Material::Material(float ra, float rd, float rs, float h) : m_ra(ra), m_rd(rd), m_rs(rs), m_h(h), m_base(0.0f), m_hasBase(false) {}
Material::Material(const vec3& baseColor, float ra, float rd, float rs, float h) : m_ra(ra), m_rd(rd), m_rs(rs), m_h(h), m_base(baseColor), m_hasBase(true) {}

void Material::applyM(ShaderProgram* shaderProgram) const
{
    if(!shaderProgram) return;
    shaderProgram->use();

    shaderProgram->set("material.ra", m_ra);
    shaderProgram->set("material.rd", m_rd);
    shaderProgram->set("material.rs", m_rs);
    shaderProgram->set("material.h", m_h);
    if(m_hasBase) shaderProgram->set("baseColor", m_base);
}