//
// Created by aven on 08/04/2026.
//

#include "ModelObject.h"

#include <array>

#include "Cube.h"

#include "../GlmMaths.h"

ModelObject::ModelObject(const std::string &model_path, const std::string &shader_name, const glm::vec3 position, const std::string& material_name) {

    this->shader = get_shader(shader_name);
    this->three_d_model_ = std::make_unique<CThreeDModel>();

    this->model_matrix = GlmMaths::pos_to_translation(position);

    const auto c_obj_loader = std::make_unique<COBJLoader>();
    if (!c_obj_loader->LoadModel(model_path)) {
        std::cerr << "Could not find model at " << model_path << std::endl;
    }

    this->three_d_model_->ConstructModelFromOBJLoader(*c_obj_loader);

    this->three_d_model_->CalcCentrePoint();
    this->three_d_model_->CentreOnZero();
    this->three_d_model_->InitVBO(this->shader.get());

    this->material = get_material(material_name);
}

void ModelObject::draw(const glm::mat4 &view, const glm::mat4 &projection, std::tuple<glm::vec4, std::array<float, 4>, std::array<float, 4>, std::array<float, 4>> light_data) const {
    glUseProgram(shader->handle());

    glm::mat4 model_view_matrix = view * model_matrix;
    glm::mat3 normal_matrix = glm::transpose(glm::inverse(model_view_matrix));

    // location
    glUniformMatrix4fv(shader->getProjLocation(), 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(shader->getModelViewLocation(), 1, GL_FALSE, &model_view_matrix[0][0]);
    glUniformMatrix3fv(glGetUniformLocation(shader->handle(), "NormalMatrix"), 1, GL_FALSE, &normal_matrix[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader->handle(), "ViewMatrix"), 1, GL_FALSE, &view[0][0]);

    // lighting
    auto [light_pos, light_ambient, light_diffuse, light_specular] = light_data;
    light_pos = view * light_pos;
    glUniform4fv(glGetUniformLocation(shader->handle(), "LightPos"), 1, &light_pos[0]);
    glUniform4fv(glGetUniformLocation(shader->handle(), "light_ambient"), 1, light_ambient.data());
    glUniform4fv(glGetUniformLocation(shader->handle(), "light_diffuse"), 1, light_diffuse.data());
    glUniform4fv(glGetUniformLocation(shader->handle(), "light_specular"), 1, light_specular.data());
    // material
    glUniform4fv(glGetUniformLocation(shader->handle(), "material_ambient"), 1, material->material_ambient.data());
    glUniform4fv(glGetUniformLocation(shader->handle(), "material_diffuse"), 1, material->material_diffuse.data());
    glUniform4fv(glGetUniformLocation(shader->handle(), "material_specular"), 1, material->material_specular.data());
    glUniform1f(glGetUniformLocation(shader->handle(), "material_shininess"), material->material_shininess);

    this->three_d_model_->DrawElementsUsingVBO(this->shader.get());
}
