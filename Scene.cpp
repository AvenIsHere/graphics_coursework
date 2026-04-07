//
// Created by aven on 27/03/2026.
//

#include "Scene.h"

#include <GL/freeglut_std.h>

#include "glm_maths.h"

#include <cmath>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

Scene::Scene(const float screen_width, const float screen_height) {
    this->screen_width = screen_width;
    this->screen_height = screen_height;

    this->camera_pos = glm::vec3(0.0, 0.0, 0.0);
    this->view_matrix = GlmMaths::pos_to_translation(camera_pos);
    this->rotation_angle = 0.0;
    this->proj_matrix = glm::perspective(glm::radians(45.0f), screen_width / screen_height, 0.1f, 100.0f);
}

void Scene::render() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto & object : objects) {
        object->draw(view_matrix, this->proj_matrix);
    }

    glutSwapBuffers();
    glutPostRedisplay();
}

void Scene::addObject(std::unique_ptr<SceneObject> obj) {
    this->objects.push_back(std::move(obj));
}

void Scene::move(const Direction direction, float amount) {

    float x_change = 0.0f;
    float y_change = 0.0f;
    float z_change = 0.0f;

    switch (direction) {
        case FORWARDS:
            x_change = std::sin(rotation_angle) * amount;
            z_change = std::cos(rotation_angle) * amount;
            break;
        case BACKWARDS:
            x_change = -std::sin(rotation_angle) * amount;
            z_change = -std::cos(rotation_angle) * amount;
            break;
        case LEFT:
            x_change = std::cos(rotation_angle) * amount;
            z_change = std::sin(rotation_angle) * amount;
            break;
        case RIGHT:
            x_change = -std::cos(rotation_angle) * amount;
            z_change = -std::sin(rotation_angle) * amount;
            break;
    }
    const glm::vec3 translation_vec(x_change, y_change, z_change);
    this->view_matrix = this->view_matrix * glm::translate(glm::mat4(1.0f), translation_vec);
}

void Scene::update() {
    if (keys['w']) move(FORWARDS, speed);
    if (keys['s']) move(BACKWARDS, speed);
    if (keys['a']) move(LEFT, speed);
    if (keys['d']) move(RIGHT, speed);

    glutPostRedisplay();
}

void Scene::handle_input_down(unsigned char key, int x, int y) {
    keys[key] = true;
}

void Scene::handle_input_up(unsigned char key, int x, int y) {
    keys[key] = false;
}
