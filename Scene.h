//
// Created by aven on 27/03/2026.
//

#ifndef GRAPHICS_COURSEWORK_SCENE_H
#define GRAPHICS_COURSEWORK_SCENE_H
#include <memory>
#include <vector>
#include <glm/glm.hpp>

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

    void rotate(Axis axis, float rotation);

    [[nodiscard]] float get_speed() const;

private:
    std::vector<std::unique_ptr<SceneObject>> objects;
    float rotation_x;
    float rotation_y;
    float speed = 0.1f;
    glm::vec3 camera_pos{};
    glm::mat4 view_matrix{};
    glm::mat4 proj_matrix{};

    float screen_width;
    float screen_height;
};


#endif //GRAPHICS_COURSEWORK_SCENE_H