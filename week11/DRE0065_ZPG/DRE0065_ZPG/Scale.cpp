#include "Scale.h"

Scale::Scale(const vec3& scale) : scale(scale) {}

void Scale::apply(mat4& matrix) const 
{
	matrix = glm::scale(matrix, scale);
}