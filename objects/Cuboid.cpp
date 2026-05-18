// OpenGL Rollercoaster Simulation
// Copyright (C) 2026 Aven Furness
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "Cuboid.h"
#include "../GlmMaths.h"

Cuboid::Cuboid(const glm::vec3 position, const glm::vec3 dimensions, const std::string& shader_name, const std::string& material_name, const std::string& name) : MeshObject(UNIT_VERTICES, UNIT_NORMALS, UNIT_CONN, shader_name, material_name, name) {
    this->aabb_dimensions = dimensions;
    const glm::mat4 model = GlmMaths::pos_to_translation(position);
    this->model_matrix = glm::scale(model, dimensions / (UNIT_SIZE * 2));

    this-> position = position - dimensions / 2.0f;
}

glm::vec3 Cuboid::get_aabb_dimensions() {
    return aabb_dimensions;
}

bool Cuboid::colliding(const glm::vec3 start_point, const glm::vec3 end_point) const {
    const glm::vec3 cuboid_start = position;
    const glm::vec3 cuboid_end = position + aabb_dimensions;

    const bool collision_x = end_point.x >= cuboid_start.x &&
        cuboid_end.x >= start_point.x;
    const bool collision_y = end_point.y >= cuboid_start.y &&
        cuboid_end.y >= start_point.y;
    const bool collision_z = end_point.z >= cuboid_start.z &&
        cuboid_end.z >= start_point.z;

    return collision_x && collision_y && collision_z;
}
