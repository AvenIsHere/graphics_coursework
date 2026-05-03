//
// Created by aven on 08/04/2026.
//

#ifndef GRAPHICS_COURSEWORK_MODELOBJECT_H
#define GRAPHICS_COURSEWORK_MODELOBJECT_H

#include <memory>

#include "SceneObject.h"
#include "../3dStruct/threeDModel.h"


class ModelObject : public SceneObject {
public:
    struct Model {
        std::string model_path;
        std::string shader_name;
        std::string material_name;
    };
private:

    std::unique_ptr<CThreeDModel> three_d_model_;

    static std::map<std::string, Model> models;

public:
    explicit ModelObject(const std::string &model_path, const std::string &shader_name, glm::vec3 position, glm::vec3 scale, const std::string& material_name);
    explicit ModelObject(const std::string& model_name, glm::vec3 position, glm::vec3 dimensions);

    void draw(const glm::mat4 & view, const glm::mat4 & projection, SceneData::Light light_data) const override;

    glm::vec3 get_aabb_dimensions() override;

    static void add_model(const std::string& model_name, Model model);
    static void add_models(const std::vector<std::tuple<std::string,Model>> &given_models);
};


#endif //GRAPHICS_COURSEWORK_MODELOBJECT_H