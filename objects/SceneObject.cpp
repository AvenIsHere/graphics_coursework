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

void SceneObject::add_materials(const std::vector<std::tuple<std::string, MaterialData>>& given_materials) {
    for (const auto& material : given_materials) {
        auto [name, mat] = material;
        add_material(name, mat);
    }
}

std::shared_ptr<SceneObject::MaterialData> SceneObject::get_material(const std::string &name) {
    return materials.at(name);
}

void SceneObject::handle_lighting(const glm::mat4 & view, SceneData::Light light) const {
    light.pos = view * light.pos;
    glUniform4fv(glGetUniformLocation(shader->handle(), "LightPos"), 1, &light.pos[0]);
    glUniform4fv(glGetUniformLocation(shader->handle(), "light_ambient"), 1, light.ambient.data());
    glUniform4fv(glGetUniformLocation(shader->handle(), "light_diffuse"), 1, light.diffuse.data());
    glUniform4fv(glGetUniformLocation(shader->handle(), "light_specular"), 1, light.specular.data());
}

void SceneObject::handle_material() const {
    glUniform4fv(glGetUniformLocation(shader->handle(), "material_ambient"), 1, material->material_ambient.data());
    glUniform4fv(glGetUniformLocation(shader->handle(), "material_diffuse"), 1, material->material_diffuse.data());
    glUniform4fv(glGetUniformLocation(shader->handle(), "material_specular"), 1, material->material_specular.data());
    glUniform1f(glGetUniformLocation(shader->handle(), "material_shininess"), material->material_shininess);
}

void SceneObject::handle_location(const glm::mat4 & view, const glm::mat4 & projection) const {
    glm::mat4 model_view_matrix = view * model_matrix;
    glm::mat3 normal_matrix = glm::transpose(glm::inverse(model_view_matrix));
    glUniformMatrix4fv(shader->getProjLocation(), 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(shader->getModelViewLocation(), 1, GL_FALSE, &model_view_matrix[0][0]);
    glUniformMatrix3fv(glGetUniformLocation(shader->handle(), "NormalMatrix"), 1, GL_FALSE, &normal_matrix[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader->handle(), "ViewMatrix"), 1, GL_FALSE, &view[0][0]);
}

