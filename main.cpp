#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <glm/glm.hpp>

#include <array>
#include <fstream>
#include <iostream>
#include <functional>
#include <vector>

#include "CoasterTrack.h"
#include "InputManager.h"
#include "Scene.h"
#include "objects/Cuboid.h"
#include "objects/ModelObject.h"

namespace Application {
    constexpr int SCREEN_WIDTH = 1000;
    constexpr int SCREEN_HEIGHT = 800;
    int time_elapsed = 0;
    int prev_time_elapsed = 0;
    std::unique_ptr<Scene> scene = nullptr;

    json get_json(const std::string& path) {
        std::ifstream f(path);
        return json::parse(f);
    }

    void run() {
        glutMainLoop();
    }

    void update() {
        time_elapsed = glutGet(GLUT_ELAPSED_TIME);
        scene->update(time_elapsed);
        InputManager::update(time_elapsed);
        prev_time_elapsed = time_elapsed;
    }

    void render() {
        scene->render();
    }

    void screen_resize(const int width, const int height) {
        scene->screen_resize(width, height);
    }

    void init(int argc, char** argv, int screen_width, int screen_height, const std::string& scene_config) {
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

        glutReshapeFunc(screen_resize);

        glutIdleFunc(update);

        scene = std::make_unique<Scene>(screen_width, screen_height, get_json(scene_config));
    }
}

int main(const int argc, char** argv) {
    Application::init(argc, argv, Application::SCREEN_WIDTH, Application::SCREEN_HEIGHT, "scene_config.json");

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
        {"grass", {
        {0.0f, 0.6f, 0.1f, 1.0f},
        {0.8f, 0.8f, 0.8f, 1.0f},
        {0.5f, 0.5f, 0.5f, 1.0f},
        5}},
        {"model", {
        {0.444f, 0.444f, 0.5f, 1.0f},
        {0.8f, 0.8f, 0.9f, 0.1f},
        {0.9f, 0.9f, 0.8f, 1.0f},
        50}}
    });

    ModelObject::add_models({
        {"rollercoaster_cart",{"rollercoaster_models/carts/red.obj", "BasicView", "model"}},
        {"STRAIGHT",{"rollercoaster_models/tracks/straight.obj", "BasicView", "model"}},
        {"BEND",{"rollercoaster_models/tracks/bend.obj", "BasicView", "model"}},
        {"LOOP",{"rollercoaster_models/tracks/loop.obj", "BasicView", "model"}}
    });

    CoasterTrack::new_tracks({
    {CoasterTrack::TrackType::STRAIGHT, {"STRAIGHT", 0.0f, {0, 0.65, 1.37}, {6.38, 0.65, 1.37}}},
    {CoasterTrack::TrackType::LEFT, {"BEND", glm::radians(90.0f)}},
    {CoasterTrack::TrackType::RIGHT, {"BEND", glm::radians(-90.0f), {0, 0.65, 1.36}, {2.38, 0.65, 3.74}}},
    {CoasterTrack::TrackType::LOOP, {"LOOP", 0.0f, {0, 0.65, 1.37}, {12.73, 0.65, 6.24}}}
    });

    auto coaster = std::make_shared<CoasterTrack>(CoasterTrack({
        CoasterTrack::TrackType::STRAIGHT,
        CoasterTrack::TrackType::STRAIGHT,
        CoasterTrack::TrackType::STRAIGHT,
        CoasterTrack::TrackType::LOOP,
        CoasterTrack::TrackType::RIGHT,
        CoasterTrack::TrackType::RIGHT,
        CoasterTrack::TrackType::STRAIGHT,
        CoasterTrack::TrackType::STRAIGHT,
        CoasterTrack::TrackType::STRAIGHT,
        CoasterTrack::TrackType::STRAIGHT,
        CoasterTrack::TrackType::STRAIGHT,
        CoasterTrack::TrackType::STRAIGHT,
        CoasterTrack::TrackType::STRAIGHT,
        CoasterTrack::TrackType::RIGHT,
        CoasterTrack::TrackType::STRAIGHT,
        CoasterTrack::TrackType::STRAIGHT,
        CoasterTrack::TrackType::RIGHT,
        CoasterTrack::TrackType::LOOP,
    }, {-20, 1, -60}));

    Application::scene->add_coaster(coaster);

    auto cart = std::make_shared<ModelObject>("rollercoaster_cart", glm::vec3(0, 12, 20), glm::vec3(1,1,1));

    Application::scene->add_objects({
        std::make_shared<Cuboid>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(500, 500, 500), "BasicView", "sky"),
        std::make_shared<Cuboid>(glm::vec3(0, -5, 0), glm::vec3(200.0, 1.0, 200.0), "BasicView", "grass"),
        cart
    });

    Application::scene->set_on_update([&](int time_elapsed) {
        cart->move(glm::vec3(0.05f, 0, 0));
        cart->rotate(0.02, glm::vec3(0, 1, 0));
    });

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
        {27, []{glutLeaveMainLoop();}},

        // edit track
        {'1', [coaster]{Application::scene->add_track_to_coaster(coaster, CoasterTrack::TrackType::STRAIGHT);}},
        {'2', [coaster]{Application::scene->add_track_to_coaster(coaster, CoasterTrack::TrackType::LEFT);}},
        {'3', [coaster]{Application::scene->add_track_to_coaster(coaster, CoasterTrack::TrackType::RIGHT);}},
        {'4', [coaster]{Application::scene->add_track_to_coaster(coaster, CoasterTrack::TrackType::LOOP);}},
        {8, [coaster]{Application::scene->pop_track_from_coaster(coaster);}}
    });

    Application::run();

    return 0;
}