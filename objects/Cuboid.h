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

#ifndef GRAPHICS_COURSEWORK_CUBE_H
#define GRAPHICS_COURSEWORK_CUBE_H
#include <vector>

#include "MeshObject.h"


class Cuboid : public MeshObject {
public:
    explicit Cuboid(glm::vec3 position, glm::vec3 dimensions, const std::string& shader_name, const std::string& material_name, const std::string& name);

    glm::vec3 get_aabb_dimensions() override;

    [[nodiscard]] bool colliding(glm::vec3 start_point, glm::vec3 end_point) const override;
private:

    //CONSTS
    static constexpr int TRI_VERTS = 3;
    static constexpr int NO_TRIS = 12;

    //UNIT CONSTS
    static inline float UNIT_SIZE = 1.0f;
    static inline std::vector<float> UNIT_VERTICES = {

        -UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,
        -UNIT_SIZE,  UNIT_SIZE, -UNIT_SIZE,
         UNIT_SIZE,  UNIT_SIZE, -UNIT_SIZE,
         UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,

        -UNIT_SIZE, -UNIT_SIZE,  UNIT_SIZE,
        -UNIT_SIZE,  UNIT_SIZE,  UNIT_SIZE,
         UNIT_SIZE,  UNIT_SIZE,  UNIT_SIZE,
         UNIT_SIZE, -UNIT_SIZE,  UNIT_SIZE
    };
    static inline std::vector<float> UNIT_NORMALS = UNIT_VERTICES;
    static inline std::vector<unsigned int> UNIT_CONN = {
        0, 1, 2,
        0, 2, 3,
        4, 6, 5,
        4, 7, 6,
        1, 5, 6,
        1, 6, 2,
        0, 7, 4,
        0, 3, 7,
        0, 5, 1,
        0, 4, 5,
        3, 2, 7,
        2, 6, 7,
    };

};


#endif //GRAPHICS_COURSEWORK_CUBE_H