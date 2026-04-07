//
// Created by aven on 27/03/2026.
//

#include "Scene.h"

#include <GL/freeglut_std.h>

#include "glm_maths.h"

#include <glm/ext/matrix_clip_space.hpp>

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
