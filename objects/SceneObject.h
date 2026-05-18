// OpenGL Rollercoaster Simulation
// Copyright (C) 2026 Aven Furness
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef GRAPHICS_COURSEWORK_SCENEOBJECT_H
#define GRAPHICS_COURSEWORK_SCENEOBJECT_H

#include <array>
#include <memory>
#include <unordered_map>
#include <vector>
#include <glm/mat4x4.hpp>

#include "../Shader.h"
#include "../Light.h"

class SceneObject {
public:
    struct MaterialData {
        std::array<float, 4> material_ambient;
        std::array<float, 4> material_diffuse;
        std::array<float, 4> material_specular;
        float material_shininess;
    };

    explicit SceneObject(std::string given_name);

    // Destructor
    virtual ~SceneObject() = default;

    virtual void draw(const glm::mat4 & view, const glm::mat4 & projection, Light light_data) const = 0;
    void move(glm::vec3 direction);
    void rotate(float angle, glm::vec3 axis);

    virtual glm::vec3 get_aabb_dimensions() = 0;

    [[nodiscard]] virtual bool colliding(glm::vec3 start_point, glm::vec3 end_point) const;

    void handle_lighting(const glm::mat4 & view, Light light) const;
    void handle_material() const;
    void handle_location(const glm::mat4 & view, const glm::mat4 & projection) const;
    [[nodiscard]] glm::mat4 handle_rotation(glm::mat4 model_view_matrix) const;

    static void add_shader(const std::string& name, const std::string &vert_path, const std::string &frag_path);
    static void add_shaders(const std::vector<std::array<std::string, 3>>& given_array);
    static std::shared_ptr<Shader> get_shader(const std::string &name);

    static void add_material(const std::string& name, MaterialData material);
    static void add_materials(const std::vector<std::tuple<std::string, MaterialData>>& given_materials);
    static std::shared_ptr<MaterialData> get_material(const std::string& name);

    std::shared_ptr<Shader> shader;
    std::shared_ptr<MaterialData> material;
    std::string name;
    glm::mat4 model_matrix;
    glm::vec3 rotation{};
protected:
    static std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
    static std::unordered_map<std::string, std::shared_ptr<MaterialData>> materials;

    glm::vec3 aabb_dimensions;

    glm::vec3 position;
};


#endif //GRAPHICS_COURSEWORK_SCENEOBJECT_H