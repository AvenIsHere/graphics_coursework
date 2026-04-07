//
// Created by aven on 27/03/2026.
//

#ifndef GRAPHICS_COURSEWORK_CUBE_H
#define GRAPHICS_COURSEWORK_CUBE_H
#include <vector>
#include <GL/glew.h>

#include "SceneObject.h"


class Cube : public SceneObject {
public:
    explicit Cube(glm::vec3 pos_vec, float side_size);
    ~Cube();

    void draw(const glm::mat4& camera_pos, const glm::mat4& projection) const override;

private:
    GLuint m_vaoID = 0;          // Vertex array object
    GLuint m_vboID[2] = {0, 0};  // two VBOs - used for colours and vertex data
    GLuint ibo = 0;
    float side_size;

    void init_buffers();
    void init_shader() const;

    //CONSTS
    static constexpr int TRI_VERTS = 3;
    static constexpr int NO_TRIS = 12;
    static constexpr int CUBE_VERTS = 8;
    static constexpr int CUBE_TRIS = 12;
    static constexpr int numOfValuesPerVertex = 3;

    //UNIT CONSTS
    float UNIT_SIZE = 1.0f;
    std::vector<float> UNIT_VERTICES = {

        -UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,
        -UNIT_SIZE,  UNIT_SIZE, -UNIT_SIZE,
         UNIT_SIZE,  UNIT_SIZE, -UNIT_SIZE,
         UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,

        -UNIT_SIZE, -UNIT_SIZE,  UNIT_SIZE,
        -UNIT_SIZE,  UNIT_SIZE,  UNIT_SIZE,
         UNIT_SIZE,  UNIT_SIZE,  UNIT_SIZE,
         UNIT_SIZE, -UNIT_SIZE,  UNIT_SIZE
    };
    std::vector<float> UNIT_COLOUR = {
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 1.0, 1.0,

        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 1.0, 0.0,
    };
    std::vector<unsigned int> UNIT_CONN = {
        0, 1, 2,
        0, 2, 3,
        4, 6, 5,
        4, 7, 6,
        1, 5, 6,
        1, 6, 2,
        0, 7, 4,
        0, 3, 7,
        0, 5, 1,
        0, 4, 5,
        3, 2, 7,
        2, 6, 7,
    };

};


#endif //GRAPHICS_COURSEWORK_CUBE_H