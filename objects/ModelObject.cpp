//
// Created by aven on 08/04/2026.
//

#include "ModelObject.h"

#include <array>

#include "Cuboid.h"

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

void ModelObject::draw(const glm::mat4 &view, const glm::mat4 &projection, SceneData::Light light_data) const {
    glUseProgram(shader->handle());
    glUniform1i(glGetUniformLocation(shader->handle(), "useTexture"), true);

    handle_location(view, projection);
    handle_lighting(view, light_data);
    handle_material();

    this->three_d_model_->DrawElementsUsingVBO(this->shader.get());
}
