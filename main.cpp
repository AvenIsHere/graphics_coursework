#include <array>
#include <fstream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <glm/glm.hpp>
#include <iostream>
#include <functional>
#include <vector>

#include "InputManager.h"
#include "Scene.h"
#include "objects/Cuboid.h"
#include "objects/ModelObject.h"

class Application {
public:
    static inline float SCREEN_WIDTH = 1000.0;
    static inline float SCREEN_HEIGHT = 800.0;
    static inline std::unique_ptr<Scene> scene = nullptr;

    static void init(int argc, char** argv, float screen_width, float screen_height, const std::string& scene_config) {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        glutCreateWindow("Test");

        glewExperimental = GL_TRUE;
        if (const GLenum err = glewInit(); GLEW_OK != err) {
            std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        }

        glutDisplayFunc(render);

        glutKeyboardFunc(InputManager::handle_input_down);
        glutKeyboardUpFunc(InputManager::handle_input_up);

        glutSpecialFunc(InputManager::handle_input_down);
        glutSpecialUpFunc(InputManager::handle_input_up);

        glutIdleFunc(update);

        scene = std::make_unique<Scene>(screen_width, screen_height, get_json(scene_config));
    }

    static json get_json(const std::string& path) {
        std::ifstream f(path);
        return json::parse(f);
    }

    static void run() {
        glutMainLoop();
    }
    static void update() {
        scene->update();
        InputManager::update();
    }

    static void render() {
        scene->render();
    }

};

int main(int argc, char** argv) {

    Application::init(argc, argv, Application::SCREEN_WIDTH, Application::SCREEN_HEIGHT, "scene_config.json");

    InputManager::add_mappings({
        // movement
        {'w', []{Application::scene->move(Scene::FORWARDS, Application::scene->get_speed());}},
        {'s', []{Application::scene->move(Scene::BACKWARDS, Application::scene->get_speed());}},
        {'a', []{Application::scene->move(Scene::LEFT, Application::scene->get_speed());}},
        {'d', []{Application::scene->move(Scene::RIGHT, Application::scene->get_speed());}},
        {' ', []{Application::scene->move(Scene::UP, Application::scene->get_speed());}},
        {GLUT_KEY_SHIFT_L, []{Application::scene->move(Scene::DOWN, Application::scene->get_speed());}, true},

        // rotation
        {GLUT_KEY_LEFT, []{Application::scene->rotate(Scene::X, -0.02f);}, true},
        {GLUT_KEY_RIGHT, []{Application::scene->rotate(Scene::X, 0.02f);}, true},
        {GLUT_KEY_UP, []{Application::scene->rotate(Scene::Y, -0.013f);}, true},
        {GLUT_KEY_DOWN, []{Application::scene->rotate(Scene::Y, 0.013f);}, true},

        // exit on esc
        {27, []{glutLeaveMainLoop();}}
    });

    SceneObject::add_shaders({
        {"BasicView", "glsl_files/basicTransformations.vert", "glsl_files/basicTransformations.frag"},
        {"Cuboid", "glsl_files/basic.vert", "glsl_files/basic.frag"}
    });

    SceneObject::add_materials({
        {"sky", {
        {0.529f, 0.808f, 0.922f, 1.0f},
        {0.529f, 0.808f, 0.922f, 1.0f},
        {0.1f, 0.08f, 0.05f, 1.0f},
        0}},
        {"grass", {{0.0f, 0.6f, 0.1f, 1.0f},
        {0.8f, 0.8f, 0.8f, 1.0f},
        {0.5f, 0.5f, 0.5f, 1.0f},
            5}},
        {"model", {{0.444f, 0.444f, 0.5f, 1.0f},
        {0.8f, 0.8f, 0.9f, 0.1f},
        {0.9f, 0.9f, 0.8f, 1.0f},
        50}}
    });

    ModelObject::add_models({
        {"rollercoaster", {"rollercoaster_models/carts/white-front.obj", "BasicView", "model"}}
    });

    Application::scene->add_objects(
        std::make_unique<Cuboid>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(500, 500, 500), "BasicView", "sky"),
        std::make_unique<Cuboid>(glm::vec3(0, -5, 0), glm::vec3(200.0, 1.0, 200.0), "BasicView", "grass"),
        std::make_unique<ModelObject>("rollercoaster", glm::vec3(0, 12, 20), glm::vec3(1,1,1))
    );

    Application::run();

    return 0;
}