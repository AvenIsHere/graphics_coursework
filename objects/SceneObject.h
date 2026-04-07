//
// Created by aven on 27/03/2026.
//

#ifndef GRAPHICS_COURSEWORK_SCENEOBJECT_H
#define GRAPHICS_COURSEWORK_SCENEOBJECT_H
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "../Shader.h"


class SceneObject {
public:
    // Destructor
    virtual ~SceneObject() = default;
    virtual void draw(const glm::mat4& view, const glm::mat4& projection) const = 0;

    Shader *shader;
    glm::vec3 pos;
    glm::mat4 model_matrix;
};


#endif //GRAPHICS_COURSEWORK_SCENEOBJECT_H