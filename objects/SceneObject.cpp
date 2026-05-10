//
// Created by aven on 27/03/2026.
//

#include "SceneObject.h"

#include <array>
#include <iostream>
#include <vector>
#include <glm/ext/matrix_transform.hpp>

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

void SceneObject::handle_lighting(const glm::mat4 & view, Scene::Light light) const {
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

glm::mat4 SceneObject::handle_rotation(glm::mat4 model_view_matrix) const {
    model_view_matrix = glm::rotate(model_view_matrix, rotation.x, glm::vec3(1,0,0));
    model_view_matrix = glm::rotate(model_view_matrix, rotation.y, glm::vec3(0, 1, 0));
    model_view_matrix = glm::rotate(model_view_matrix, rotation.z, glm::vec3(0, 0, 1));
    return model_view_matrix;
}

void SceneObject::handle_location(const glm::mat4 & view, const glm::mat4 & projection) const {
    glm::mat4 rotated_model = handle_rotation(model_matrix);
    glm::mat4 model_view_matrix = view * rotated_model;
    glm::mat3 normal_matrix = glm::transpose(glm::inverse(model_view_matrix));
    glUniformMatrix4fv(shader->getProjLocation(), 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(shader->getModelViewLocation(), 1, GL_FALSE, &model_view_matrix[0][0]);
    glUniformMatrix3fv(glGetUniformLocation(shader->handle(), "NormalMatrix"), 1, GL_FALSE, &normal_matrix[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader->handle(), "ViewMatrix"), 1, GL_FALSE, &view[0][0]);
}

void SceneObject::move(const glm::vec3 direction) {
    glm::mat4 rotate(1.0f);
    rotate = glm::rotate(rotate, rotation.x, glm::vec3(1,0,0));
    rotate = glm::rotate(rotate, rotation.y, glm::vec3(0,1,0));
    rotate = glm::rotate(rotate, rotation.z, glm::vec3(0,0,1));
    const auto rotate_matrix = glm::vec3(rotate * glm::vec4(direction, 0.0f));
    position += rotate_matrix;
    this->model_matrix = glm::translate(this->model_matrix, rotate_matrix);
}

void SceneObject::rotate(const float angle, const glm::vec3 axis) {
    rotation += angle * axis;
    for (int i = 0; i < glm::vec3::length(); i++) {
        rotation[i] = std::fmod(rotation[i], static_cast<float>(2*std::numbers::pi));
    }
    move({0,0,0});
}

bool SceneObject::colliding(glm::vec3 obj_start, glm::vec3 obj_end) const {
    const auto this_start = position;
    const auto this_end = position + aabb_dimensions;
    return ((this_start.x <= obj_end.x && this_end.x >= obj_start.x) &&
        (this_start.y <= obj_end.y && this_end.y >= obj_start.y) &&
        (this_start.z <= obj_end.z && this_end.z >= obj_start.z));
}
