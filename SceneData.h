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

    struct Light {
        glm::vec4 pos;
        std::array<float, 4> ambient;
        std::array<float, 4> diffuse;
        std::array<float, 4> specular;
    };

    SceneData(int screen_width, int screen_height, json json_data);

    // camera
    float speed;
    std::unordered_map<std::string, float> rotation;
    glm::vec3 camera_pos{};
    glm::mat4 view_matrix{};
    glm::mat4 proj_matrix{};

    Light light{};
};


#endif //GRAPHICS_COURSEWORK_SCENEDATA_H