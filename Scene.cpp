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

#include "objects/Cuboid.h"

Scene::Scene(const int screen_width, const int screen_height, json json_data) : scene_config(screen_width, screen_height, std::move(json_data)) {
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

void Scene::add_object(const std::shared_ptr<SceneObject>& obj) {
    this->objects.push_back(obj);
}

void Scene::add_objects(const std::vector<std::shared_ptr<SceneObject>>& given_objects) {
    for (const auto& object : given_objects) {
        add_object(object);
    }
}

void Scene::remove_object(const std::shared_ptr<SceneObject> &obj) {
    if (const auto item = std::ranges::find(this->objects, obj); item != this->objects.end()) {
        this->objects.erase(item);
    }
}

void Scene::remove_objects(const std::vector<std::shared_ptr<SceneObject> > &given_objects) {
    for (const auto& obj : given_objects) {
        remove_object(obj);
    }
}

void Scene::add_coaster(const std::shared_ptr<CoasterTrack>& coaster) {
    coasters.emplace_back(coaster);
    add_objects(coaster->get_model_objs());
}

void Scene::remove_coaster(const std::shared_ptr<CoasterTrack> &coaster) {
    if (const auto& item = std::ranges::find(coasters, coaster); item != coasters.end()) {
        remove_objects(coaster->get_model_objs());
        coasters.erase(item);
    }
}

void Scene::add_track_to_coaster(const std::shared_ptr<CoasterTrack> &coaster, CoasterTrack::TrackType type) {
    if (const auto& item = std::ranges::find(coasters, coaster); item != coasters.end()) {
        add_object(coaster->add_track(type));
    }
}

void Scene::pop_track_from_coaster(const std::shared_ptr<CoasterTrack> &coaster) {
    if (const auto& item = std::ranges::find(coasters, coaster); item != coasters.end()) {
        remove_object(coaster->pop_track());
    }
}

void Scene::load_coaster_from_file(const std::shared_ptr<CoasterTrack> &coaster) {
    if (const auto& item = std::ranges::find(coasters, coaster); item != coasters.end()) {
        remove_objects(coaster->get_model_objs());
        add_objects(coaster->load_from_file());
    }
}

void Scene::set_on_update(const std::function<void(int)> &func) {
    on_update = func;
}

void Scene::move(const Direction direction, const float amount) {
    const float x_dir = std::sin(scene_config.rotation["x"]);
    const float z_dir = -std::cos(scene_config.rotation["x"]);

    const glm::vec3 prev_pos = scene_config.camera_pos;

    switch (direction) {
        case FORWARDS:
            scene_config.camera_pos.x += x_dir * amount;
            scene_config.camera_pos.z += z_dir * amount;
            break;
        case BACKWARDS:
            scene_config.camera_pos.x -= x_dir * amount;
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

    for (const auto& obj : objects) {
        if (obj->colliding(scene_config.camera_pos - glm::vec3{0.1}, scene_config.camera_pos + glm::vec3{0.1})) {
            scene_config.camera_pos = prev_pos;
        }
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

void Scene::update_view(int time_elapsed) {
    auto view = glm::mat4(1.0f);
    view = glm::rotate(view, scene_config.rotation["y"], glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, scene_config.rotation["x"], glm::vec3(0.0f, 1.0f, 0.0f));
    scene_config.view_matrix = glm::translate(view, -scene_config.camera_pos);
}

void Scene::update(const int time_elapsed) {
    glutPostRedisplay();
    update_view(time_elapsed);
    if (on_update) on_update(time_elapsed);
}

float Scene::get_speed() const {
    return scene_config.speed;
}

void Scene::screen_resize(const int width, const int height) {
    const float aspect = static_cast<float>(width) / static_cast<float>(height);
    scene_config.proj_matrix = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 1000.0f);
    glViewport(0, 0, width, height);
}
