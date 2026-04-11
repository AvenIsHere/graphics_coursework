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

    Scene(float screen_width, float screen_height);

    void update();
    void update_view();

    void render() const;

    void addObject(std::unique_ptr<SceneObject> obj);
    void move(Direction direction, float amount);

    void rotate(Axis axis, float given_rotation);

    [[nodiscard]] float get_speed() const;

private:
    std::vector<std::unique_ptr<SceneObject>> objects;

    SceneData scene_config;

    float screen_width;
    float screen_height;
};


#endif //GRAPHICS_COURSEWORK_SCENE_H