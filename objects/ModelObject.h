//
// Created by aven on 08/04/2026.
//

#ifndef GRAPHICS_COURSEWORK_MODELOBJECT_H
#define GRAPHICS_COURSEWORK_MODELOBJECT_H
#include <array>
#include <memory>

#include "SceneObject.h"
#include "../3dStruct/threeDModel.h"


class ModelObject : public SceneObject {
    std::unique_ptr<CThreeDModel> three_d_model_;

    std::array<float, 4> material_ambient{};
    std::array<float, 4> material_diffuse{};
    std::array<float, 4> material_specular{};
    float material_shininess{};

public:
    explicit ModelObject(const std::string &model_path, const std::string &shader_name, glm::vec3 position, const std::tuple<std::array<float, 4>, std::array<float, 4>, std::array<float, 4>, float> &material_data);
    void draw(const glm::mat4 & view, const glm::mat4 & projection, std::tuple<glm::vec4, std::array<float, 4>, std::array<float, 4>> light_data) const override;
};


#endif //GRAPHICS_COURSEWORK_MODELOBJECT_H