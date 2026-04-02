//
// Created by aven on 27/03/2026.
//

#ifndef GRAPHICS_COURSEWORK_SCENE_H
#define GRAPHICS_COURSEWORK_SCENE_H
#include <memory>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>

#include "objects/SceneObject.h"


class Scene {
public:
    Scene(float screen_width, float screen_height);
    void update();
    void render() const;
    void handleInput(unsigned char key, int x, int y);
    void addObject(std::unique_ptr<SceneObject> obj);
private:
    std::vector<std::unique_ptr<SceneObject>> objects;
    float rotation_angle;
    glm::vec3 camera_pos{};
    glm::mat4 view_matrix{};
    glm::mat4 proj_matrix{};

    float screen_width;
    float screen_height;
};


#endif //GRAPHICS_COURSEWORK_SCENE_H