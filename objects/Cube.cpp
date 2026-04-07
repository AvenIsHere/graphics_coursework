//
// Created by aven on 27/03/2026.
//

#include "Cube.h"

#include <iostream>
#include <vector>
#include <GL/glew.h>

#include "../glm_maths.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

Cube::Cube(const glm::vec3 pos_vec, const float side_size) {
    this->pos = pos_vec;
    this->model_matrix = GlmMaths::pos_to_translation(this->pos);
    this->shader = new Shader;
    this->side_size = side_size;

    init_shader();
    init_buffers();
}

Cube::~Cube() {
    glDeleteBuffers(2, m_vboID);
    glDeleteBuffers(1, &ibo);

    glDeleteVertexArrays(1, &m_vaoID);

    if (shader != nullptr) {
        delete shader;
        shader = nullptr;
    }
}

void Cube::init_buffers() {
    // VAO allocation
    glGenVertexArrays(1, &m_vaoID);

    // First VAO setup
    glBindVertexArray(m_vaoID);

    glGenBuffers(2, m_vboID);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboID[0]);
    //initialises data storage of vertex buffer object
    glBufferData(GL_ARRAY_BUFFER, CUBE_VERTS* numOfValuesPerVertex *sizeof(GLfloat), UNIT_VERTICES.data(), GL_STATIC_DRAW);
    const GLint vertexLocation= glGetAttribLocation(shader->handle(), "in_Position");
    glVertexAttribPointer(vertexLocation, numOfValuesPerVertex, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(vertexLocation);


    glBindBuffer(GL_ARRAY_BUFFER, m_vboID[1]);
    glBufferData(GL_ARRAY_BUFFER, CUBE_VERTS* numOfValuesPerVertex *sizeof(GLfloat), UNIT_COLOUR.data(), GL_STATIC_DRAW);
    const GLint colorLocation= glGetAttribLocation(shader->handle(), "in_Color");
    glVertexAttribPointer(colorLocation, numOfValuesPerVertex, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(colorLocation);


    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, CUBE_TRIS * TRI_VERTS * sizeof(unsigned int), UNIT_CONN.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Cube::init_shader() const {
    if (!shader->load("Basic", "./glslfiles/basicTransformations.vert", "./glslfiles/basicTransformations.frag"))
    {
        std::cout << "failed to load shader" << std::endl;
    }
}

void Cube::draw(const glm::mat4& camera_pos, const glm::mat4& projection) const {
    glUseProgram(shader->handle());
    glBindVertexArray(m_vaoID);

    glm::mat4 model_view_matrix = camera_pos * this->model_matrix;

    glUniformMatrix4fv(this->shader->getProjLocation(), 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(this->shader->getModelViewLocation(), 1, GL_FALSE, &model_view_matrix[0][0]);

    glDrawElements(GL_TRIANGLES, NO_TRIS * TRI_VERTS, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
    glUseProgram(0);
}
