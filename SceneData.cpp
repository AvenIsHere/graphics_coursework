//
// Created by aven on 11/04/2026.
//

#include "SceneData.h"

#include <glm/ext/matrix_clip_space.hpp>

#include "GlmMaths.h"


SceneData::SceneData(float screen_width, float screen_height, json json_data) {

    this->json_data = json_data;

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
    light.pos = glm::vec4(
        json_data["light-pos"]["x"],
        json_data["light-pos"]["y"],
        json_data["light-pos"]["z"],
        json_data["light-pos"]["w"]);
    for (int i = 0; i < 4; i++) {
        light.ambient[i] = json_data["light-ambient"][i];
        light.diffuse[i] = json_data["light-diffuse"][i];
        light.specular[i] = json_data["light-specular"][i];
    }
}
