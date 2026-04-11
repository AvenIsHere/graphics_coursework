#include <array>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <iostream>

#include "InputManager.h"
#include "Scene.h"
#include "objects/Cube.h"
#include "objects/ModelObject.h"

class Application {
public:
    static inline float SCREEN_WIDTH = 1000.0;
    static inline float SCREEN_HEIGHT = 800.0;
    static inline std::unique_ptr<Scene> scene = nullptr;
    static inline std::unique_ptr<InputManager> input_manager = nullptr;

    static void init(int argc, char** argv, float screen_width, float screen_height) {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        glutCreateWindow("Test");

        glewExperimental = GL_TRUE;
        if (const GLenum err = glewInit(); GLEW_OK != err) {
            std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        }

        glutDisplayFunc(render);

        glutKeyboardFunc(handle_input_down);
        glutKeyboardUpFunc(handle_input_up);

        glutSpecialFunc(handle_input_down);
        glutSpecialUpFunc(handle_input_up);

        glutIdleFunc(update);

        scene = std::make_unique<Scene>(screen_width, screen_height);
        input_manager = std::make_unique<InputManager>(scene.get());
    }
    static void run() {
        glutMainLoop();
    }
    static void update() {
        if (scene && input_manager) {
            scene->update();
            input_manager->update();
        }
    }

    static void render() {
        if (scene) {
            scene->render();
        }
    }

    static void handle_input_down(unsigned char key, int x, int y) {
        if (input_manager) {
            input_manager->handle_input_down(key, x, y);
        }
    }

    static void handle_input_down(int key, int x, int y) {
        if (input_manager) {
            input_manager->handle_input_down(key, x, y);
        }
    }

    static void handle_input_up(unsigned char key, int x, int y) {
        if (input_manager) {
            input_manager->handle_input_up(key, x, y);
        }
    }

    static void handle_input_up(int key, int x, int y) {
        if (input_manager) {
            input_manager->handle_input_up(key, x, y);
        }
    }

};

int main(int argc, char** argv) {

    Application::init(argc, argv, Application::SCREEN_WIDTH, Application::SCREEN_HEIGHT);

    SceneObject::add_shaders({
        {"BasicView", "glslfiles/basicTransformations.vert", "glslfiles/basicTransformations.frag"},
        {"Cube", "glslfiles/basic.vert", "glslfiles/basic.frag"}
    });

    Application::scene->addObject(std::make_unique<Cube>(glm::vec3(0, 0, -5), 2.0f));
    Application::scene->addObject(std::make_unique<ModelObject>("TestModels/axes.obj", "BasicView",glm::vec3(0, 5, 5)));

    Application::run();

    return 0;
}