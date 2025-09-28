#ifndef SCENE_H
#define SCENE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "DrawableObject.h"

class Scene
{
    public:
        Scene();
        ~Scene();
        void draw();

    private:
        Model* model = nullptr;
        ShaderProgram* shaderProgram = nullptr;
        DrawableObject* object = nullptr;

		Model* model2 = nullptr;
		ShaderProgram* shaderProgram2 = nullptr;
		DrawableObject* object2 = nullptr;
};
#endif