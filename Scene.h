//
// Created by aven on 27/03/2026.
//

#ifndef GRAPHICS_COURSEWORK_SCENE_H
#define GRAPHICS_COURSEWORK_SCENE_H
#include <memory>
#include <vector>
#include <nlohmann/json.hpp>

#include "SceneData.h"
using json = nlohmann::json;

#include "objects/SceneObject.h"


class Scene {
public:
    enum Direction {
        FORWARDS,
        BACKWARDS,
        LEFT,
        RIGHT,
        UP,
        DOWN,
    };

    enum Axis {
        X,
        Y,
    };

    Scene(int screen_width, int screen_height, json json_data);

    void update();
    void update_view();
    void render() const;

    void add_object(std::shared_ptr<SceneObject> obj);
    template<typename... Args>
    void add_objects(Args&&... object) {
        (add_object(std::forward<Args>(object)), ...);
    }

    void set_on_update(const std::function<void()> &func);

    void move(Direction direction, float amount);
    void rotate(Axis axis, float given_rotation);

    void screen_resize(int width, int height);

    [[nodiscard]] float get_speed() const;

private:
    std::vector<std::shared_ptr<SceneObject>> objects;

    std::function<void()> on_update;

    SceneData scene_config;

    int screen_width;
    int screen_height;
};


#endif //GRAPHICS_COURSEWORK_SCENE_H