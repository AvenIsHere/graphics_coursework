//
// Created by aven on 11/04/2026.
//

#include "SceneData.h"

#include <fstream>
#include <glm/ext/matrix_clip_space.hpp>

#include "glm_maths.h"

SceneData::SceneData(float screen_width, float screen_height) {

    std::ifstream f("scene_config.json");
    json_data = json::parse(f);

    // camera
    camera_pos = glm::vec3(
        json_data["camera-pos"]["x"],
        json_data["camera-pos"]["y"],
        json_data["camera-pos"]["z"]);
    rotation = {
        {"x", json_data["camera-rotation"]["x"]},
        {"y", json_data["camera-rotation"]["y"]}
    };
    speed = json_data["camera-speed"];
    view_matrix = GlmMaths::pos_to_translation(camera_pos);
    proj_matrix = glm::perspective(glm::radians(45.0f), screen_width / screen_height, 0.1f, 100.0f);

    // lighting
    light_pos = glm::vec4(
        json_data["light-pos"]["x"],
        json_data["light-pos"]["y"],
        json_data["light-pos"]["z"],
        json_data["light-pos"]["w"]);
}
