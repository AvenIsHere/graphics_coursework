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

#ifndef GRAPHICS_COURSEWORK_LIGHT_H
#define GRAPHICS_COURSEWORK_LIGHT_H
#include <array>
#include <glm/vec4.hpp>

struct Light {
    glm::vec4 pos;
    std::array<float, 4> ambient;
    std::array<float, 4> diffuse;
    std::array<float, 4> specular;
};

#endif //GRAPHICS_COURSEWORK_LIGHT_H
