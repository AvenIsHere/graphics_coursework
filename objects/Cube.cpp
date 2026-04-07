//
// Created by aven on 27/03/2026.
//

#include "Cube.h"

#include <iostream>
#include <GL/glew.h>

#include "../glm_maths.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>



Cube::Cube(const glm::vec3 pos_vec) {
    this->pos = pos_vec;
    this->model_matrix = pos_to_translation(this->pos);
    this->shader = new Shader;
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
