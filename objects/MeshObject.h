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
        const std::string& material_name,
        const std::string &name);
    ~MeshObject() override;

    void draw(const glm::mat4 & view, const glm::mat4 & projection, Light light_data) const override;

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