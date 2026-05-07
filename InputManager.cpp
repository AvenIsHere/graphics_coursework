//
// Created by aven on 08/04/2026.
//

#include <cctype>
#include <utility>

#include "InputManager.h"

std::unordered_map<int, bool> InputManager::hold_keys;
std::map<int, std::function<void()>> InputManager::hold_functions;
std::map<int, std::function<void()>> InputManager::tap_functions;

void InputManager::handle_input_down(unsigned char key, int x, int y) {
    if (std::isupper(key)) key = std::tolower(key);
    hold_keys[key] = true;
    if (tap_functions.count(key)) tap_functions[key]();
}

void InputManager::handle_input_up(unsigned char key, int x, int y) {
    if (std::isupper(key)) key = std::tolower(key);
    hold_keys[key] = false;
}

void InputManager::handle_input_down(const int key, int x, int y) {
    hold_keys[key + 256] = true;
    if (tap_functions.count(key + 256)) tap_functions[key + 256]();
}

void InputManager::handle_input_up(const int key, int x, int y) {
    hold_keys[key + 256] = false;
}

void InputManager::update(int time_elapsed) {
    for (const auto&[key, func] : hold_functions) {
        if (hold_keys[key]) {
            func();
        }
    }

}

void InputManager::add_hold_mapping(int key, std::function<void()> func, bool special) {
    if (special) key += 256;
    hold_functions[key] = std::move(func);
}

void InputManager::add_hold_mappings(const std::vector<InputMapping>& mappings) {
    for (const auto& [key, func, special] : mappings) {
        add_hold_mapping(key, func, special);
    }
}

void InputManager::add_tap_mapping(int key, std::function<void()> func, bool special) {
    if (special) key += 256;
    tap_functions[key] = std::move(func);
}

void InputManager::add_tap_mappings(const std::vector<InputMapping> &mappings) {
    for (const auto& [key, func, special] : mappings) {
        add_tap_mapping(key, func, special);
    }
}
