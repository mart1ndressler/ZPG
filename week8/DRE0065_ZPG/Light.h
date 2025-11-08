#pragma once
#include <glm/glm.hpp>
#include "Subject.h"

using namespace glm;

class Light : public Subject
{
    public:
        Light(const vec3& position);
        vec3 getPosition() const;
        void setPosition(const vec3& p);

    private:
        vec3 position;
};