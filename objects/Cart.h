//
// Created by aven on 11/05/2026.
//

#ifndef GRAPHICS_COURSEWORK_CART_H
#define GRAPHICS_COURSEWORK_CART_H
#include <memory>

#include "ModelObject.h"


class Cart : public ModelObject {
public:
    Cart(glm::vec3 initial_position, const std::string& cart_model, std::string name);
};


#endif //GRAPHICS_COURSEWORK_CART_H
