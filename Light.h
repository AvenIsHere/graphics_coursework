//
// Created by aven on 10/05/2026.
//

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
