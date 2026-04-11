//
// Created by aven on 08/04/2026.
//

#include "InputManager.h"

#include <random>
#include <utility>

#include "Scene.h"
#include "objects/Cube.h"

std::map<int, std::function<void()>> InputManager::functions;

void InputManager::handle_input_down(unsigned char key, int x, int y) {
    key = std::tolower(key);
    keys[key] = true;
}

void InputManager::handle_input_up(unsigned char key, int x, int y) {
    key = std::tolower(key);
    keys[key] = false;
}

void InputManager::handle_input_down(int key, int x, int y) {
    keys[key + 256] = true;
}

void InputManager::handle_input_up(int key, int x, int y) {
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
    functions.emplace(key, func);
}

void InputManager::add_mappings(const std::vector<InputMapping>& mappings) {
    for (const auto& mapping : mappings) {
        auto [key, func, special] = mapping;
        if (special) key += 256;
        functions.emplace(key, func);
    }
}
