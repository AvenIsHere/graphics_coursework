//
// Created by aven on 08/04/2026.
//

#include "ModelObject.h"

#include "Cuboid.h"

#include "../GlmMaths.h"

std::map<std::string, ModelObject::Model> ModelObject::models;

ModelObject::ModelObject(const std::string &model_path, const std::string &shader_name, const glm::vec3 position, const glm::vec3 scale, const std::string& material_name) {

    this->shader = get_shader(shader_name);
    this->three_d_model_ = std::make_unique<CThreeDModel>();

    this->model_matrix = GlmMaths::pos_to_translation(position);
    this->model_matrix = glm::scale(model_matrix, scale);

    const auto c_obj_loader = std::make_unique<COBJLoader>();
    if (!c_obj_loader->LoadModel(model_path)) {
        std::cerr << "Could not find model at " << model_path << std::endl;
    }

    this->three_d_model_->ConstructModelFromOBJLoader(*c_obj_loader);

    this->three_d_model_->CalcCentrePoint();
    this->three_d_model_->CentreOnZero();
    this->three_d_model_->InitVBO(this->shader.get());

    this->material = get_material(material_name);

    this->aabb_dimensions = get_aabb_dimensions();
}

ModelObject::ModelObject(const std::string &model_name, const glm::vec3 position, const glm::vec3 dimensions) :
    ModelObject(models.at(model_name).model_path,
        models.at(model_name).shader_name,
        position,
        dimensions,
        models.at(model_name).material_name){
}

glm::vec3 ModelObject::get_aabb_dimensions() {
    double min_x, max_x, min_y, max_y, min_z, max_z;
    three_d_model_->CalcBoundingBox(min_x, min_y, min_z, max_x, max_y, max_z);
    return {max_x-min_x, max_y-min_y, max_z-min_z};
}

void ModelObject::draw(const glm::mat4 &view, const glm::mat4 &projection, SceneData::Light light_data) const {
    glUseProgram(shader->handle());
    glUniform1i(glGetUniformLocation(shader->handle(), "useTexture"), true);

    handle_location(view, projection);
    handle_lighting(view, light_data);
    handle_material();

    this->three_d_model_->DrawElementsUsingVBO(this->shader.get());
}

void ModelObject::add_model(const std::string& model_name, Model model) {
    models[model_name] = std::move(model);
}

void ModelObject::add_models(const std::vector<std::tuple<std::string,Model>> &given_models) {
    for (const auto& [mod_name, mod] : given_models) {
        models[mod_name] = mod;
    }
}
