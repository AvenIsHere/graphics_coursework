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
    enum Direction {
        FORWARDS,
        BACKWARDS,
        LEFT,
        RIGHT,
    };

public:
    Scene(float screen_width, float screen_height);

    void update();
    void update_view();

    void render() const;

    //handle ASCII input
    void handle_input_down(unsigned char key, int x, int y);
    void handle_input_up(unsigned char key, int x, int y);

    //handle special character input
    void handle_input_down(int key, int x, int y);
    void handle_input_up(int key, int x, int y);

    void addObject(std::unique_ptr<SceneObject> obj);
    void move(Direction direction, float amount);

    void rotate(float rotation);

private:
    std::vector<std::unique_ptr<SceneObject>> objects;
    float rotation_angle;
    float speed = 0.1f;
    glm::vec3 camera_pos{};
    glm::mat4 view_matrix{};
    glm::mat4 proj_matrix{};

    bool keys[512] = { false };

    float screen_width;
    float screen_height;
};


#endif //GRAPHICS_COURSEWORK_SCENE_H