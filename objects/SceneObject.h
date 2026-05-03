//
// Created by aven on 27/03/2026.
//

#ifndef GRAPHICS_COURSEWORK_SCENEOBJECT_H
#define GRAPHICS_COURSEWORK_SCENEOBJECT_H

#include <array>
#include <memory>
#include <unordered_map>
#include <vector>
#include <glm/mat4x4.hpp>

#include "../Shader.h"
#include "../SceneData.h"

class SceneObject {
public:
    struct MaterialData {
        std::array<float, 4> material_ambient;
        std::array<float, 4> material_diffuse;
        std::array<float, 4> material_specular;
        float material_shininess;
    };

    // Destructor
    virtual ~SceneObject() = default;

    virtual void draw(const glm::mat4 & view, const glm::mat4 & projection, SceneData::Light light_data) const = 0;
    void move(glm::vec3 direction);
    void rotate(float angle, glm::vec3 axis);

    virtual glm::vec3 get_aabb_dimensions() = 0;


    void handle_lighting(const glm::mat4 & view, SceneData::Light light) const;
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
    glm::mat4 model_matrix;
    glm::vec3 rotation{};
protected:
    static std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
    static std::unordered_map<std::string, std::shared_ptr<MaterialData>> materials;

    glm::vec3 aabb_dimensions;
};


#endif //GRAPHICS_COURSEWORK_SCENEOBJECT_H