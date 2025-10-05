#include "TransformationComposite.h"

void TransformationComposite::addComponent(TransformationComponent* component) {components.push_back(component);}
void TransformationComposite::apply(mat4& matrix) const {for (const auto& component : components) component->apply(matrix);}
void TransformationComposite::clear() {components.clear();}
TransformationComposite::~TransformationComposite() {for (auto component : components) delete component;}