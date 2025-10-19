#include "Light.h"

Light::Light(const vec3& position) : position(position) {}

vec3 Light::getPosition() const
{
    return position;
}

void Light::setPosition(const vec3& p)
{
    position = p;
    pushToPrograms();
}

void Light::attach(ShaderProgram* program)
{
    if(!program) return;
    if(find(programs.begin(), programs.end(), program) == programs.end())
    {
        programs.push_back(program);
        pushTo(program);
    }
}

void Light::detach(ShaderProgram* program)
{
    auto it = find(programs.begin(), programs.end(), program);
    if(it != programs.end()) programs.erase(it);
}

void Light::pushToPrograms() const
{
    for(auto* p : programs) if(p) pushTo(p);
}

void Light::pushTo(ShaderProgram* program) const
{
    program->use();
    program->set("lightPosition", position);
}