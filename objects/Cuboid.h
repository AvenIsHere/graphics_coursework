//
// Created by aven on 27/03/2026.
//

#ifndef GRAPHICS_COURSEWORK_CUBE_H
#define GRAPHICS_COURSEWORK_CUBE_H
#include <vector>

#include "MeshObject.h"


class Cuboid : public MeshObject {
public:
    explicit Cuboid(glm::vec3 position, glm::vec3 dimensions, const std::string& shader_name, const std::string& material_name);

    glm::vec3 get_aabb_dimensions() override;
private:

    //CONSTS
    static constexpr int TRI_VERTS = 3;
    static constexpr int NO_TRIS = 12;

    //UNIT CONSTS
    static inline float UNIT_SIZE = 1.0f;
    static inline std::vector<float> UNIT_VERTICES = {

        -UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,
        -UNIT_SIZE,  UNIT_SIZE, -UNIT_SIZE,
         UNIT_SIZE,  UNIT_SIZE, -UNIT_SIZE,
         UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,

        -UNIT_SIZE, -UNIT_SIZE,  UNIT_SIZE,
        -UNIT_SIZE,  UNIT_SIZE,  UNIT_SIZE,
         UNIT_SIZE,  UNIT_SIZE,  UNIT_SIZE,
         UNIT_SIZE, -UNIT_SIZE,  UNIT_SIZE
    };
    static inline std::vector<float> UNIT_NORMALS = UNIT_VERTICES;
    static inline std::vector<unsigned int> UNIT_CONN = {
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