//
// Created by aven on 08/04/2026.
//

#include <cctype>
#include <utility>

#include "InputManager.h"

std::unordered_map<int, bool> InputManager::keys;
std::map<int, std::function<void()>> InputManager::functions;

void InputManager::handle_input_down(unsigned char key, int x, int y) {
    if (std::isupper(key)) key = std::tolower(key);
    keys[key] = true;
}

void InputManager::handle_input_up(unsigned char key, int x, int y) {
    if (std::isupper(key)) key = std::tolower(key);
    keys[key] = false;
}

void InputManager::handle_input_down(const int key, int x, int y) {
    keys[key + 256] = true;
}

void InputManager::handle_input_up(const int key, int x, int y) {
    keys[key + 256] = false;
}

void InputManager::update() {
    for (const auto&[key, func] : functions) {
        if (keys[key]) {
            func();
        }
    }

}

void InputManager::add_to_map(int key, std::function<void()> func, bool special) {
    if (special) key += 256;
    functions[key] = std::move(func);
}

void InputManager::add_mappings(const std::vector<InputMapping>& mappings) {
    for (const auto& mapping : mappings) {
        auto [key, func, special] = mapping;
        add_to_map(key, func, special);
    }
}
