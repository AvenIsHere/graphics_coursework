#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <iostream>

#include "Scene.h"
#include "objects/Cube.h"

std::unique_ptr<Scene> scene;

float SCREEN_WIDTH = 500.0;
float SCREEN_HEIGHT = 350.0;

void render() {
    if (scene) {
        scene->render();
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

    scene->addObject(std::make_unique<Cube>(glm::vec3(0, 0, -5), 2.0f));

    glutDisplayFunc(render);
    glutMainLoop();

    return 0;
}