//
// Created by aven on 27/03/2026.
//

#ifndef GRAPHICS_COURSEWORK_SCENEOBJECT_H
#define GRAPHICS_COURSEWORK_SCENEOBJECT_H

#include <memory>
#include <unordered_map>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "../Shader.h"


class SceneObject {
protected:
    static std::unordered_map<std::string, std::array<std::string, 2>> shaders;
public:
    // Destructor
    virtual ~SceneObject() = default;
    virtual void draw(const glm::mat4 & view, const glm::mat4 & projection, std::tuple<glm::vec4, std::array<float, 4>, std::array<float, 4>> light_data) const = 0;

    static void add_shader(const std::string& name, const std::string &vert_path, const std::string &frag_path);
    static void add_shaders(const std::vector<std::array<std::string, 3>>& given_array);
    static std::unique_ptr<Shader> get_shader(const std::string &name);

    std::unique_ptr<Shader> shader;
    glm::mat4 model_matrix;
};


#endif //GRAPHICS_COURSEWORK_SCENEOBJECT_H