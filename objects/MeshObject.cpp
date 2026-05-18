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

#include "MeshObject.h"

#include <memory>

MeshObject::MeshObject(const std::vector<float> &vertices, const std::vector<float>& normals, const std::vector<unsigned> &indices, const std::string &shader_name, const std::string& material_name, const std::string &name) : SceneObject(name) {
    this->shader = get_shader(shader_name);
    this->material = get_material(material_name);
    indices_size = indices.size();
    init_buffers(vertices, normals, indices);
}

MeshObject::~MeshObject() {
    glDeleteBuffers(3, m_vboID);
    glDeleteBuffers(1, &m_iboID);
    glDeleteVertexArrays(1, &m_vaoID);
}

void MeshObject::init_buffers(const std::vector<float> &vertices, const std::vector<float>& normals, const std::vector<unsigned> &indices) {
    // VAO allocation
    glGenVertexArrays(1, &m_vaoID);

    // First VAO setup
    glBindVertexArray(m_vaoID);

    glGenBuffers(3, m_vboID);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboID[0]);
    //initialises data storage of vertex buffer object
    glBufferData(GL_ARRAY_BUFFER, vertices.size() *sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    const GLint vertex_location = glGetAttribLocation(shader->handle(), "in_Position");
    glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(vertex_location);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboID[2]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() *sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);
    const GLint normal_location = glGetAttribLocation(shader->handle(), "in_Normal");
    glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(normal_location);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &m_iboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void MeshObject::draw(const glm::mat4 & view, const glm::mat4 & projection, const Light light_data) const {
    glUseProgram(shader->handle());
    glBindVertexArray(m_vaoID);
    glUniform1i(glGetUniformLocation(shader->handle(), "useTexture"), false);

    handle_location(view, projection);
    handle_lighting(view, light_data);
    handle_material();

    glDrawElements(GL_TRIANGLES, indices_size, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
    glUseProgram(0);
}
