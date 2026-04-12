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

public:
    explicit ModelObject(const std::string &model_path, const std::string &shader_name, glm::vec3 position, const std::string& material_name);
    void draw(const glm::mat4 & view, const glm::mat4 & projection, SceneData::Light light_data) const override;
};


#endif //GRAPHICS_COURSEWORK_MODELOBJECT_H