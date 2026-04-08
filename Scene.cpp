//
// Created by aven on 27/03/2026.
//

#include "Scene.h"

#include <GL/freeglut_std.h>

#include "glm_maths.h"

#include <cmath>
#include <random>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "objects/Cube.h"

Scene::Scene(const float screen_width, const float screen_height) {
    this->screen_width = screen_width;
    this->screen_height = screen_height;
    glutReshapeWindow(screen_width, screen_height);

    this->camera_pos = glm::vec3(0.0, 0.0, 0.0);
    this->light_pos = glm::vec4(10.0f, 10.0f, 10.0f, 1.0f);
    this->view_matrix = GlmMaths::pos_to_translation(camera_pos);
    this->rotation_x = 0.0;
    this->rotation_y = 0.0;
    this->proj_matrix = glm::perspective(glm::radians(45.0f), screen_width / screen_height, 0.1f, 100.0f);
}

void Scene::render() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto & object : objects) {
        object->draw(view_matrix, this->proj_matrix, light_pos);
    }

    glutSwapBuffers();
}

void Scene::addObject(std::unique_ptr<SceneObject> obj) {
    this->objects.push_back(std::move(obj));
}

void Scene::move(const Direction direction, float amount) {

    float x_dir = std::sin(rotation_x) * std::cos(rotation_y);
    float y_dir = -std::sin(rotation_y);
    float z_dir = -std::cos(rotation_x) * std::cos(rotation_y);

    switch (direction) {
        case FORWARDS:
            camera_pos.x += x_dir * amount;
            camera_pos.y += y_dir * amount;
            camera_pos.z += z_dir * amount;
            break;
        case BACKWARDS:
            camera_pos.x -= x_dir * amount;
            camera_pos.y -= y_dir * amount;
            camera_pos.z -= z_dir * amount;
            break;
        case LEFT:
            camera_pos.x -= std::cos(rotation_x) * amount;
            camera_pos.z -= std::sin(rotation_x) * amount;
            break;
        case RIGHT:
            camera_pos.x += std::cos(rotation_x) * amount;
            camera_pos.z += std::sin(rotation_x) * amount;
            break;
        case UP:
            camera_pos.y += amount;
            break;
        case DOWN:
            camera_pos.y -= amount;
    }
}

void Scene::rotate(const Axis axis, const float rotation) {
    switch (axis) {
        case X:
            rotation_x += rotation;
            break;
        case Y:
            rotation_y += rotation;
            break;
    }
}

void Scene::update_view() {
    auto view = glm::mat4(1.0f);
    view = glm::rotate(view, rotation_y, glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, rotation_x, glm::vec3(0.0f, 1.0f, 0.0f));
    this->view_matrix = glm::translate(view, -camera_pos);
}

void Scene::update() {
    glutPostRedisplay();
    update_view();
}

float Scene::get_speed() const {
    return speed;
}