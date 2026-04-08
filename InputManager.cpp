//
// Created by aven on 08/04/2026.
//

#include "InputManager.h"

#include <random>
#include <GL/freeglut_std.h>
#include <GL/freeglut_ext.h>

#include "Scene.h"
#include "objects/Cube.h"

InputManager::InputManager(Scene *scene) {
    this->scene = scene;
}

void InputManager::handle_input_down(unsigned char key, int x, int y) {
    keys[key] = true;
}

void InputManager::handle_input_up(unsigned char key, int x, int y) {
    keys[key] = false;
}

void InputManager::handle_input_down(int key, int x, int y) {
    keys[key + 256] = true;
}

void InputManager::handle_input_up(int key, int x, int y) {
    keys[key + 256] = false;
}

void InputManager::update() const {

    // movement
    if (keys['w']) scene->move(Scene::FORWARDS, scene->get_speed());
    if (keys['s']) scene->move(Scene::BACKWARDS, scene->get_speed());
    if (keys['a']) scene->move(Scene::LEFT, scene->get_speed());
    if (keys['d']) scene->move(Scene::RIGHT, scene->get_speed());
    if (keys[' ']) scene->move(Scene::UP, scene->get_speed());
    if (keys[GLUT_KEY_SHIFT_L + 256]) scene->move(Scene::DOWN, scene->get_speed());

    // rotation
    if (keys[GLUT_KEY_LEFT + 256]) scene->rotate(Scene::X, -0.02f);
    if (keys[GLUT_KEY_RIGHT + 256]) scene->rotate(Scene::X, 0.02f);
    if (keys[GLUT_KEY_UP + 256]) scene->rotate(Scene::Y, -0.02f);
    if (keys[GLUT_KEY_DOWN + 256]) scene->rotate(Scene::Y, 0.02f);

    // exit on esc
    if (keys[27]) glutLeaveMainLoop();

    // add cube
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> loc(1, 40);
    std::uniform_int_distribution<> siz(1, 6);
    if (keys['e']) scene->addObject(std::make_unique<Cube>(glm::vec3(loc(gen), loc(gen), loc(gen)), siz(gen)));

}
