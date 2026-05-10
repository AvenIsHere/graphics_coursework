//
// Created by aven on 07/04/2026.
//

#ifndef GRAPHICS_COURSEWORK_MESHOBJECT_H
#define GRAPHICS_COURSEWORK_MESHOBJECT_H
#include <vector>

#include "SceneObject.h"


class MeshObject : public SceneObject {
public:
    MeshObject(const std::vector<float>& vertices,
        const std::vector<float>& normals,
        const std::vector<unsigned int>& indices,
        const std::string& shader_name,
        const std::string& material_name);
    ~MeshObject() override;

    void draw(const glm::mat4 & view, const glm::mat4 & projection, Scene::Light light_data) const override;

protected:
    GLuint m_vaoID = 0;
    GLuint m_vboID[3] = {0, 0, 0};
    GLuint m_iboID = 0;

    size_t indices_size;

    void init_buffers(const std::vector<float>& vertices,
        const std::vector<float>& normals,
        const std::vector<unsigned int>& indices);
};


#endif //GRAPHICS_COURSEWORK_MESHOBJECT_H