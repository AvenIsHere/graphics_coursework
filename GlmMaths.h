//
// Created by aven on 02/04/2026.
//

#ifndef GRAPHICS_COURSEWORK_GLM_MATHS_H
#define GRAPHICS_COURSEWORK_GLM_MATHS_H
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>


class GlmMaths {
public:
    static glm::mat4 pos_to_translation(const glm::vec3& vec) {
        return glm::translate(glm::mat4(1.0f), vec);
    }
};


#endif //GRAPHICS_COURSEWORK_GLM_MATHS_H