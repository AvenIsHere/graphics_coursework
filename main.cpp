#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <glm/glm.hpp>

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

    void update(int value) {
        time_elapsed = glutGet(GLUT_ELAPSED_TIME);
        scene->update(time_elapsed);
        InputManager::update(time_elapsed);
        prev_time_elapsed = time_elapsed;

        glutTimerFunc(16, update, 0);
    }

    void render() {
        scene->render();
    }

    void screen_resize(const int width, const int height) {
        scene->screen_resize(width, height);
    }

    void load_shaders_json(const std::string &shaders_folder) {
        for (const auto& file : std::filesystem::directory_iterator(shaders_folder)) {
            json shader_json = get_json(file.path());
            SceneObject::add_shader(shader_json["name"], shader_json["vert_path"], shader_json["frag_path"]);
        }
    }

    void load_materials_json(const std::string &materials_folder) {
        for (const auto& file : std::filesystem::directory_iterator(materials_folder)) {
            json material_json = get_json(file.path());
            SceneObject::add_material(material_json["name"], {material_json["material_ambient"], material_json["material_diffuse"], material_json["material_specular"], material_json["material_shininess"]});
        }
    }

    void load_models_json(const std::string &models_folder) {
        for (const auto& file : std::filesystem::directory_iterator(models_folder)) {
            json model = get_json(file.path());
            ModelObject::add_model(model["name"], {model["model_path"], model["shader_name"], model["material_name"]});
        }
    }

    void load_tracks_json(const std::string &tracks_file) {
        json tracks_json = get_json(tracks_file);
        for (const auto& track : tracks_json["tracks"]) {
            CoasterTrack::new_track({track["name"], CoasterTrack::TrackData{track["piece_model"], glm::radians(static_cast<float>(track["rotation"])), {track["start_point"][0], track["start_point"][1], track["start_point"][2]}, {track["end_point"][0], track["end_point"][1], track["end_point"][2]}}});
        }
    }

    void load_objects_json(const std::string &objects_file) {
        json objects_json = get_json(objects_file);
        for (const auto& object : objects_json["objects"]) {
            if (object["obj_type"] == "Cuboid") {
                scene->add_object(std::make_shared<Cuboid>(glm::vec3{object["position"][0], object["position"][1], object["position"][2]}, glm::vec3{object["dimensions"][0], object["dimensions"][1], object["dimensions"][2]}, object["shader_name"], object["material_name"], object["name"]));
                continue;
            }
            if (object["obj_type"] == "Model") {
                scene-> add_object(std::make_shared<ModelObject>(object["model_name"], glm::vec3{object["position"][0], object["position"][1], object["position"][2]}, glm::vec3{object["dimensions"][0], object["dimensions"][1], object["dimensions"][2]}, object["name"]));
            }
        }
    }

    void load_from_json(const std::string& shaders_folder, const std::string& materials_folder, const std::string& models_folder, const std::string &tracks_folder, const std::string &objects_json) {
        load_shaders_json(shaders_folder);
        load_materials_json(materials_folder);
        load_models_json(models_folder);
        load_tracks_json(tracks_folder);
        load_objects_json(objects_json);
    }

    std::shared_ptr<CoasterTrack> load_default_coaster() {
        return std::make_shared<CoasterTrack>("config/default_coaster.json", glm::vec3{-20, 1, -60}, get_json("config/default_coaster.json")["cart_model"]);
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

        glutTimerFunc(0, update, 0);

        scene = std::make_unique<Scene>(screen_width, screen_height, get_json(scene_config));
    }
}

int main(const int argc, char** argv) {
    Application::init(argc, argv, Application::SCREEN_WIDTH, Application::SCREEN_HEIGHT, "config/scene_config.json");
    Application::load_from_json("config/shaders", "config/materials", "config/models", "config/tracks.json", "config/objects.json");

    auto coaster = Application::load_default_coaster();
    Application::scene->add_coaster(coaster);

    InputManager::add_hold_mappings({
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
    });

    InputManager::add_tap_mappings({
        // edit track
        {'1', [coaster]{Application::scene->add_track_to_coaster(coaster, "straight");}},
        {'2', [coaster]{Application::scene->add_track_to_coaster(coaster, "left");}},
        {'3', [coaster]{Application::scene->add_track_to_coaster(coaster, "right");}},
        {'4', [coaster]{Application::scene->add_track_to_coaster(coaster, "loop");}},
        {'5', [coaster]{Application::scene->add_track_to_coaster(coaster, "step_up");}},
        {'6', [coaster]{Application::scene->add_track_to_coaster(coaster, "step_down");}},
        {8, [coaster]{Application::scene->pop_track_from_coaster(coaster);}},

        {'p', [coaster]{coaster->save_to_file();}},
        {'l', [coaster]{Application::scene->load_coaster_from_file(coaster);}}
    });

    Application::run();

    return 0;
}