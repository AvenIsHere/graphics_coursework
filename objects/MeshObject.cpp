//
// Created by aven on 07/04/2026.
//

#include "MeshObject.h"

#include <iostream>
#include <memory>

MeshObject::MeshObject(const std::vector<float> &vertices, const std::vector<float> &colours, const std::vector<unsigned> &indices, const std::string &shader_name) {

    this->shader = std::make_unique<Shader>();
    if (!shader->load(shader_name, "./glslfiles/basicTransformations.vert", "./glslfiles/basicTransformations.frag"))
    {
        std::cerr << "failed to load shader" << std::endl;
    }
    indices_size = indices.size();
    init_buffers(vertices, colours, indices);
}

MeshObject::~MeshObject() {
    glDeleteBuffers(2, m_vboID);
    glDeleteBuffers(1, &m_iboID);
    glDeleteVertexArrays(1, &m_vaoID);
}

void MeshObject::init_buffers(const std::vector<float> &vertices, const std::vector<float> &colours, const std::vector<unsigned> &indices) {
    // VAO allocation
    glGenVertexArrays(1, &m_vaoID);

    // First VAO setup
    glBindVertexArray(m_vaoID);

    glGenBuffers(2, m_vboID);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboID[0]);
    //initialises data storage of vertex buffer object
    glBufferData(GL_ARRAY_BUFFER, vertices.size() *sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    const GLint vertexLocation= glGetAttribLocation(shader->handle(), "in_Position");
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(vertexLocation);


    glBindBuffer(GL_ARRAY_BUFFER, m_vboID[1]);
    glBufferData(GL_ARRAY_BUFFER, colours.size() *sizeof(GLfloat), colours.data(), GL_STATIC_DRAW);
    const GLint colorLocation= glGetAttribLocation(shader->handle(), "in_Color");
    glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(colorLocation);


    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &m_iboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void MeshObject::draw(const glm::mat4 &view, const glm::mat4 &projection) const {
    glUseProgram(shader->handle());
    glBindVertexArray(m_vaoID);

    glm::mat4 model_view_matrix = view * this->model_matrix;

    glUniformMatrix4fv(this->shader->getProjLocation(), 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(this->shader->getModelViewLocation(), 1, GL_FALSE, &model_view_matrix[0][0]);

    glDrawElements(GL_TRIANGLES, indices_size, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
    glUseProgram(0);
}
