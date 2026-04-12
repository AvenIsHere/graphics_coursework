//
// Created by aven on 27/03/2026.
//

#include "SceneObject.h"

#include <array>
#include <iostream>
#include <vector>

#include "../Scene.h"

std::unordered_map<std::string, std::shared_ptr<Shader>> SceneObject::shaders;
std::unordered_map<std::string, std::shared_ptr<SceneObject::MaterialData>> SceneObject::materials;

void SceneObject::add_shader(const std::string& name, const std::string &vert_path, const std::string &frag_path) {
    auto shader = std::make_shared<Shader>();
    if (!shader->load(name, vert_path.c_str(), frag_path.c_str())) {
        std::cerr << "Failed to load shader" << std::endl;
    }
    shaders.emplace(name, shader);
}

void SceneObject::add_shaders(const std::vector<std::array<std::string, 3>>& given_array) {
    for (auto shader : given_array) {
        add_shader(shader[0], shader[1], shader[2]);
    }
}

std::shared_ptr<Shader> SceneObject::get_shader(const std::string &name) {
    return shaders.at(name);
}

void SceneObject::add_material(const std::string &name, MaterialData material) {
    materials.emplace(name, std::make_shared<MaterialData>(material));
}

void SceneObject::add_materials(const std::vector<std::tuple<std::string, MaterialData>>& materials) {
    for (const auto& material : materials) {
        auto [name, mat] = material;
        add_material(name, mat);
    }
}

std::shared_ptr<SceneObject::MaterialData> SceneObject::get_material(const std::string &name) {
    return materials.at(name);
}


