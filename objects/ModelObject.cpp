//
// Created by aven on 08/04/2026.
//

#include "ModelObject.h"

#include <utility>

#include "Cuboid.h"

#include "../GlmMaths.h"

std::map<std::string, ModelObject::Model> ModelObject::models;

ModelObject::ModelObject(const std::string &model_path, const std::string &shader_name, const glm::vec3 position, const glm::vec3 scale, const std::string& material_name, std::string name) : SceneObject(std::move(name)) {
    this->shader = get_shader(shader_name);
    this->three_d_model_ = std::make_unique<CThreeDModel>();

    this->model_matrix = GlmMaths::pos_to_translation(position);
    this->model_matrix = glm::scale(model_matrix, scale);

    this->position = position;

    this->scale = scale;

    const auto c_obj_loader = std::make_unique<COBJLoader>();
    if (!c_obj_loader->LoadModel(model_path)) {
        std::cerr << "Could not find model at " << model_path << std::endl;
    }

    this->three_d_model_->ConstructModelFromOBJLoader(*c_obj_loader);

    this->three_d_model_->MoveOriginToMin();
    this->three_d_model_->InitVBO(this->shader.get());

    this->material = get_material(material_name);

    this->aabb_dimensions = ModelObject::get_aabb_dimensions();
}

ModelObject::ModelObject(const std::string &model_name, const glm::vec3 position, const glm::vec3 dimensions, std::string name) :
    ModelObject(models.at(model_name).model_path,
        models.at(model_name).shader_name,
        position,
        dimensions,
        models.at(model_name).material_name,
        std::move(name)){
}

glm::vec3 ModelObject::get_aabb_dimensions() {
    double min_x, max_x, min_y, max_y, min_z, max_z;
    three_d_model_->CalcBoundingBox(min_x, min_y, min_z, max_x, max_y, max_z);
    glm::vec3 return_scale = {max_x-min_x, max_y-min_y, max_z-min_z};
    return_scale *= scale;
    return return_scale;
}

void ModelObject::draw(const glm::mat4 &view, const glm::mat4 &projection, Light light_data) const {
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

bool ModelObject::colliding(glm::vec3 start_point, glm::vec3 end_point) const {
    glm::mat4 full_model_mat = handle_rotation(model_matrix);

    glm::mat4 inverse_model_mat = glm::inverse(full_model_mat);

    glm::vec4 local_start = inverse_model_mat * glm::vec4(start_point, 1.0f);
    glm::vec4 local_end = inverse_model_mat * glm::vec4(end_point, 1.0f);

    double start_double[] = {
        std::min(local_start.x, local_end.x),
        std::min(local_start.y, local_end.y),
        std::min(local_start.z, local_end.z)
    };
    double end_double[] = {
        std::max(local_start.x, local_end.x),
        std::max(local_start.y, local_end.y),
        std::max(local_start.z, local_end.z)
    };

    double center[] = {
        (start_double[0] + end_double[0]) / 2.0,
        (start_double[1] + end_double[1]) / 2.0,
        (start_double[2] + end_double[2]) / 2.0
    };

    double half_size[] = {
        (end_double[0] - start_double[0]) / 2.0,
        (end_double[1] - start_double[1]) / 2.0,
        (end_double[2] - start_double[2]) / 2.0
    };


    return three_d_model_->IsIntersecting(center, half_size);
}
