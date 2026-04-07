#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <iostream>

#include "Scene.h"
#include "objects/Cube.h"

Scene* scene;

float SCREEN_WIDTH = 500.0;
float SCREEN_HEIGHT = 350.0;

void render() {
    if (scene) {
        scene->render();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutCreateWindow("Test");

    glewExperimental = GL_TRUE;
    if (const GLenum err = glewInit(); GLEW_OK != err) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    }

    scene = new Scene(SCREEN_WIDTH, SCREEN_HEIGHT);

    scene->addObject(std::make_unique<Cube>(Cube(glm::vec3(0, -1, 0))));

    glutDisplayFunc(render);
    glutMainLoop();

    return 0;
}