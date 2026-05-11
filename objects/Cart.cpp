//
// Created by aven on 11/05/2026.
//

#include "Cart.h"

#include <utility>

Cart::Cart(glm::vec3 initial_position, const std::string& cart_model, std::string name) : ModelObject(cart_model, initial_position, {1, 1, 1}, std::move(name)) {
}
