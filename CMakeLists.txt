cmake_minimum_required(VERSION 4.1)
project(graphics_coursework)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(GLEW REQUIRED)

find_library(FREEIMAGE_LIB FreeImage)

include_directories(${GLUT_INCLUDE_DIR} ${OPENGL_INCLUDE_DIRS})

add_executable(graphics_coursework main.cpp
        Scene.cpp
        Scene.h
        objects/SceneObject.cpp
        objects/SceneObject.h
        objects/Cube.cpp
        objects/Cube.h
        glm_maths.cpp
        glm_maths.h
        Shader.h
        Shader.cpp
        objects/MeshObject.cpp
        objects/MeshObject.h
        InputManager.cpp
        InputManager.h
        3dStruct/threeDModel.h
        3dStruct/threeDModel.cpp
        TextureHandler/TextureHandler.cpp
        Structures/Vector3d.cpp
        Octree/Octree.cpp
        Box/Box.cpp
        Utilities/IntersectionTests.cpp
        Obj/OBJLoader.cpp
        Images/ImageLoading.cpp
)

target_link_libraries(graphics_coursework ${OPENGL_LIBRARIES} GLUT::GLUT glm::glm ${GLEW_LIBRARIES} freeimage)