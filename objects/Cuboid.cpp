//
// Created by aven on 27/03/2026.
//

#include "Cuboid.h"
#include "../GlmMaths.h"

Cuboid::Cuboid(const glm::vec3 position, const glm::vec3 dimensions, const std::string& shader_name, const std::string& material_name) : MeshObject(UNIT_VERTICES, UNIT_NORMALS, UNIT_CONN, shader_name, material_name) {
    this->dimensions = dimensions;
    const glm::mat4 model = GlmMaths::pos_to_translation(position);
    this->model_matrix = glm::scale(model, dimensions / (UNIT_SIZE * 2));
}
