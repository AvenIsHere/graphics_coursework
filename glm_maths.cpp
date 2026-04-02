//
// Created by aven on 02/04/2026.
//

#include "glm_maths.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

glm::mat4 pos_to_translation(const glm::vec3& vec) {
    return glm::translate(glm::mat4(1.0f), vec);
}
