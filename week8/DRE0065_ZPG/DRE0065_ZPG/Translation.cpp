#include "Translation.h"

Translation::Translation(const vec3& translation) : translation(translation) {}

void Translation::apply(mat4& matrix) const
{
	matrix = glm::translate(matrix, translation);
}