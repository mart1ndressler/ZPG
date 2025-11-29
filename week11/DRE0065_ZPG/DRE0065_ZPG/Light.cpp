#include "Light.h"

Light::Light(const vec3& position) : position(position) {}

vec3 Light::getPosition() const
{
    return position;
}

void Light::setPosition(const vec3& p)
{
    position = p;
    notifyLight(position);
}