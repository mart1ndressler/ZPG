#pragma once
#ifndef TRANSFORMATION_COMPOSITE_H
#define TRANSFORMATION_COMPOSITE_H

#include <vector>

#include "TransformationComponent.h"

using namespace glm;
using namespace std;

class TransformationComposite : public TransformationComponent
{
    public:
        void addComponent(TransformationComponent* component);
        void apply(mat4& matrix) const override;
        void clear();
        ~TransformationComposite();

    private:
        vector<TransformationComponent*> components;
};
#endif