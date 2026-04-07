//
// Created by aven on 27/03/2026.
//

#include "Scene.h"

#include <GL/freeglut_std.h>
#include <GL/freeglut_ext.h>

#include "glm_maths.h"

#include <cmath>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

Scene::Scene(const float screen_width, const float screen_height) {
    this->screen_width = screen_width;
    this->screen_height = screen_height;

    this->camera_pos = glm::vec3(0.0, 0.0, 0.0);
    this->view_matrix = GlmMaths::pos_to_translation(camera_pos);
    this->rotation_x = 0.0;
    this->rotation_y = 0.0;
    this->proj_matrix = glm::perspective(glm::radians(45.0f), screen_width / screen_height, 0.1f, 100.0f);
}

void Scene::render() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto & object : objects) {
        object->draw(view_matrix, this->proj_matrix);
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

void Scene::update_view() {
    constexpr auto identity = glm::mat4(1.0f);
    const glm::mat4 rotation_y_mat = glm::rotate(identity, rotation_y, glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 rotation_x_mat = glm::rotate(identity, rotation_x, glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 translation = glm::translate(identity, -camera_pos);
    this->view_matrix = rotation_y_mat * rotation_x_mat * translation;
}

void Scene::update() {

    // movement
    if (keys['w']) move(FORWARDS, speed);
    if (keys['s']) move(BACKWARDS, speed);
    if (keys['a']) move(LEFT, speed);
    if (keys['d']) move(RIGHT, speed);
    if (keys[' ']) move(UP, speed);
    if (keys[GLUT_KEY_SHIFT_L + 256]) move(DOWN, speed);

    // rotation
    if (keys[GLUT_KEY_LEFT + 256]) rotation_x -= 0.02f;
    if (keys[GLUT_KEY_RIGHT + 256]) rotation_x += 0.02f;
    if (keys[GLUT_KEY_UP + 256]) rotation_y -= 0.02f;
    if (keys[GLUT_KEY_DOWN + 256]) rotation_y += 0.02f;

    // exit on esc
    if (keys[27]) glutLeaveMainLoop();

    glutPostRedisplay();
    update_view();
}

void Scene::handle_input_down(unsigned char key, int x, int y) {
    keys[key] = true;
}

void Scene::handle_input_up(unsigned char key, int x, int y) {
    keys[key] = false;
}

void Scene::handle_input_down(int key, int x, int y) {
    keys[key + 256] = true;
}

void Scene::handle_input_up(int key, int x, int y) {
    keys[key + 256] = false;
}
