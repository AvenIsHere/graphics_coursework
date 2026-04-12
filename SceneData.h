//
// Created by aven on 11/04/2026.
//

#ifndef GRAPHICS_COURSEWORK_SCENEDATA_H
#define GRAPHICS_COURSEWORK_SCENEDATA_H
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <nlohmann/json.hpp>
using json = nlohmann::json;


class SceneData {
    json json_data;
public:
    SceneData(float screen_width, float screen_height, json json_data);

    std::tuple<glm::vec4, std::array<float, 4>, std::array<float, 4>, std::array<float, 4>> get_light_data() const;

    // camera
    float speed;
    std::unordered_map<std::string, float> rotation;
    glm::vec3 camera_pos{};
    glm::mat4 view_matrix{};
    glm::mat4 proj_matrix{};

    // lighting
    glm::vec4 light_pos{};
    std::array<float, 4> light_ambient{};
    std::array<float, 4> light_diffuse{};
    std::array<float, 4> light_specular{};
};


#endif //GRAPHICS_COURSEWORK_SCENEDATA_H