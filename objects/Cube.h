//
// Created by aven on 27/03/2026.
//

#ifndef GRAPHICS_COURSEWORK_CUBE_H
#define GRAPHICS_COURSEWORK_CUBE_H
#include "SceneObject.h"


class Cube : public SceneObject {
public:
    explicit Cube(glm::vec3 pos_vec, const CShader& shader);
    void draw(const glm::mat4& camera_pos, const glm::mat4& projection) const;

private:
    GLuint m_vaoID = 0; // Vertex array object
    GLuint ibo = 0;

    //CONSTS
    static constexpr int TRI_VERTS = 3;
    static constexpr int NO_TRIS = 12;
};


#endif //GRAPHICS_COURSEWORK_CUBE_H