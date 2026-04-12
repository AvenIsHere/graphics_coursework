//
// Created by aven on 27/03/2026.
//

#include "Cube.h"
#include "../GlmMaths.h"

Cube::Cube(const glm::vec3 pos_vec, const float side_size, const std::string& shader_name, const std::string& material_name) : MeshObject(UNIT_VERTICES, UNIT_COLOUR, UNIT_NORMALS, UNIT_CONN, shader_name, material_name) {
    this->model_matrix = GlmMaths::pos_to_translation(pos_vec);
    this->side_size = side_size;
}
