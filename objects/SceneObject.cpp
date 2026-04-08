//
// Created by aven on 27/03/2026.
//

#include "SceneObject.h"

#include <array>
#include <iostream>
#include <vector>

std::unordered_map<std::string, std::array<std::string, 2>> SceneObject::shaders;

void SceneObject::add_shader(const std::string& name, const std::string &vert_path, const std::string &frag_path) {
    shaders.emplace(name, std::array<std::string, 2>({vert_path, frag_path}));
}

void SceneObject::add_shaders(const std::vector<std::array<std::string, 3>>& given_array) {
    for (auto shader : given_array) {
        add_shader(shader[0], shader[1], shader[2]);
    }
}

std::unique_ptr<Shader> SceneObject::get_shader(const std::string &name) {
    auto return_shader = std::make_unique<Shader>();
    if (!return_shader->load(name, shaders.at(name)[0].c_str(), shaders.at(name)[1].c_str())) {
        std::cerr << "Failed to load shader" << std::endl;
    }
    return return_shader;
}
