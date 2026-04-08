#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <iostream>

#include "InputManager.h"
#include "Scene.h"
#include "objects/Cube.h"

std::unique_ptr<Scene> scene;
std::unique_ptr<InputManager> input_manager;

float SCREEN_WIDTH = 500.0;
float SCREEN_HEIGHT = 500.0;

void render() {
    if (scene) {
        scene->render();
    }
}

void handle_input_down(unsigned char key, int x, int y) {
    if (input_manager) {
        input_manager->handle_input_down(key, x, y);
    }
}

void handle_input_down(int key, int x, int y) {
    if (input_manager) {
        input_manager->handle_input_down(key, x, y);
    }
}

void handle_input_up(unsigned char key, int x, int y) {
    if (input_manager) {
        input_manager->handle_input_up(key, x, y);
    }
}

void handle_input_up(int key, int x, int y) {
    if (input_manager) {
        input_manager->handle_input_up(key, x, y);
    }
}

void update() {
    if (scene) {
        scene->update();
        input_manager->update();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Test");

    glewExperimental = GL_TRUE;
    if (const GLenum err = glewInit(); GLEW_OK != err) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    }

    scene = std::make_unique<Scene>(SCREEN_WIDTH, SCREEN_HEIGHT);
    input_manager = std::make_unique<InputManager>(scene.get());

    scene->addObject(std::make_unique<Cube>(glm::vec3(0, 0, -5), 2.0f));

    glutDisplayFunc(render);

    glutKeyboardFunc(handle_input_down);
    glutKeyboardUpFunc(handle_input_up);

    glutSpecialFunc(handle_input_down);
    glutSpecialUpFunc(handle_input_up);

    glutIdleFunc(update);

    glutMainLoop();

    return 0;
}