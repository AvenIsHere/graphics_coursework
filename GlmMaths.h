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

#ifndef GRAPHICS_COURSEWORK_GLM_MATHS_H
#define GRAPHICS_COURSEWORK_GLM_MATHS_H
#include <glm/mat4x4.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>


class GlmMaths {
public:

    struct MatComponents {
        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 translation;
        glm::vec3 skew;
        glm::vec4 perspective;
    };

    static glm::mat4 pos_to_translation(const glm::vec3& vec) {
        return glm::translate(glm::mat4(1.0f), vec);
    }
    static MatComponents mat_info(const glm::mat4& mat) {// your transformation matrix.
        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 translation;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(mat, scale, rotation, translation, skew, perspective);
        return MatComponents{scale, rotation, translation, skew, perspective};
    }
};


#endif //GRAPHICS_COURSEWORK_GLM_MATHS_H