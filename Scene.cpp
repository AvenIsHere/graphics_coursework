//
// Created by aven on 27/03/2026.
//

#include "Scene.h"

#include <GL/freeglut_std.h>

#include "GlmMaths.h"

#include <cmath>
#include <fstream>
#include <random>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <utility>

#include "objects/Cube.h"

Scene::Scene(const float screen_width, const float screen_height, json json_data) : scene_config(screen_width, screen_height, std::move(json_data)) {
    this->screen_width = screen_width;
    this->screen_height = screen_height;
    glutReshapeWindow(screen_width, screen_height);
}

void Scene::render() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto & object : objects) {
        object->draw(scene_config.view_matrix, scene_config.proj_matrix, scene_config.light);
    }

    glutSwapBuffers();
}

void Scene::add_object(std::unique_ptr<SceneObject> obj) {
    this->objects.push_back(std::move(obj));
}

void Scene::move(const Direction direction, float amount) {

    float x_dir = std::sin(scene_config.rotation["x"]) * std::cos(scene_config.rotation["y"]);
    float y_dir = -std::sin(scene_config.rotation["y"]);
    float z_dir = -std::cos(scene_config.rotation["x"]) * std::cos(scene_config.rotation["y"]);

    switch (direction) {
        case FORWARDS:
            scene_config.camera_pos.x += x_dir * amount;
            scene_config.camera_pos.y += y_dir * amount;
            scene_config.camera_pos.z += z_dir * amount;
            break;
        case BACKWARDS:
            scene_config.camera_pos.x -= x_dir * amount;
            scene_config.camera_pos.y -= y_dir * amount;
            scene_config.camera_pos.z -= z_dir * amount;
            break;
        case LEFT:
            scene_config.camera_pos.x -= std::cos(scene_config.rotation["x"]) * amount;
            scene_config.camera_pos.z -= std::sin(scene_config.rotation["x"]) * amount;
            break;
        case RIGHT:
            scene_config.camera_pos.x += std::cos(scene_config.rotation["x"]) * amount;
            scene_config.camera_pos.z += std::sin(scene_config.rotation["x"]) * amount;
            break;
        case UP:
            scene_config.camera_pos.y += amount;
            break;
        case DOWN:
            scene_config.camera_pos.y -= amount;
    }
}

void Scene::rotate(const Axis axis, const float given_rotation) {
    switch (axis) {
        case X:
            scene_config.rotation["x"] += given_rotation;
            break;
        case Y:
            scene_config.rotation["y"] += given_rotation;
            break;
    }
}

void Scene::update_view() {
    auto view = glm::mat4(1.0f);
    view = glm::rotate(view, scene_config.rotation["y"], glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, scene_config.rotation["x"], glm::vec3(0.0f, 1.0f, 0.0f));
    scene_config.view_matrix = glm::translate(view, -scene_config.camera_pos);
}

void Scene::update() {
    glutPostRedisplay();
    update_view();
}

float Scene::get_speed() const {
    return scene_config.speed;
}