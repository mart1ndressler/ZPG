#include "Rotation.h"

Rotation::Rotation(float angle, const vec3& axis) : angle(angle), axis(axis) {}
void Rotation::apply(mat4& matrix) const {matrix = glm::rotate(matrix, angle, axis);}