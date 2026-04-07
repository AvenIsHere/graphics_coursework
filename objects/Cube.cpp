//
// Created by aven on 27/03/2026.
//

#include "Cube.h"
#include "../glm_maths.h"

Cube::Cube(const glm::vec3 pos_vec, const float side_size) : MeshObject(UNIT_VERTICES, UNIT_COLOUR, UNIT_CONN, "Basic") {
    this->pos = pos_vec;
    this->model_matrix = GlmMaths::pos_to_translation(this->pos);
    this->side_size = side_size;
}
