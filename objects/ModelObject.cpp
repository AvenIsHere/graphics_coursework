//
// Created by aven on 08/04/2026.
//

#include "ModelObject.h"

#include "../glm_maths.h"

ModelObject::ModelObject(const std::string &model_path, const std::string &shader_name, const glm::vec3 position) {

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
}

void ModelObject::draw(const glm::mat4 &view, const glm::mat4 &projection, glm::vec4 light_pos) const {
    glUseProgram(shader->handle());

    glm::mat4 ModelViewMatrix = view * model_matrix;
    glm::mat3 normal_matrix = glm::transpose(glm::inverse(ModelViewMatrix));

    glUniformMatrix4fv(shader->getProjLocation(), 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(shader->getModelViewLocation(), 1, GL_FALSE, &ModelViewMatrix[0][0]);
    glUniformMatrix3fv(glGetUniformLocation(shader->handle(), "NormalMatrix"), 1, GL_FALSE, &normal_matrix[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader->handle(), "ViewMatrix"), 1, GL_FALSE, &view[0][0]);
    glUniform4fv(glGetUniformLocation(shader->handle(), "LightPos"), 1, &light_pos[0]);

    this->three_d_model_->DrawElementsUsingVBO(this->shader.get());
}
