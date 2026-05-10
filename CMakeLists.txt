cmake_minimum_required(VERSION 4.1)
project(graphics_coursework)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(GLEW REQUIRED)

find_library(FREEIMAGE_LIB FreeImage)

include(FetchContent)

FetchContent_Declare(
        magic_enum
        GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
        GIT_TAG        v0.9.8
)
FetchContent_MakeAvailable(magic_enum)

FetchContent_Declare(
        nativefiledialog-extended
        GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended
        GIT_TAG        v1.3.0
)
FetchContent_MakeAvailable(nativefiledialog-extended)

include_directories(${GLUT_INCLUDE_DIR} ${OPENGL_INCLUDE_DIRS})

add_executable(graphics_coursework main.cpp
        Scene.cpp
        Scene.h
        objects/SceneObject.cpp
        objects/SceneObject.h
        objects/Cuboid.cpp
        objects/Cuboid.h
        GlmMaths.h
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
        objects/ModelObject.cpp
        objects/ModelObject.h
        objects/ModelObject.cpp
        CoasterTrack.cpp
        CoasterTrack.h
)

target_link_libraries(graphics_coursework ${OPENGL_LIBRARIES} GLUT::GLUT glm::glm ${GLEW_LIBRARIES} freeimage magic_enum::magic_enum nfd)

add_custom_target(copy-files
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/glsl_files"
        "${CMAKE_CURRENT_BINARY_DIR}/glsl_files"
)

add_custom_target(rollercoaster-models
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/rollercoaster_models"
        "${CMAKE_CURRENT_BINARY_DIR}/rollercoaster_models"
)

add_custom_command(
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/config/scene_config.json"
        COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_CURRENT_SOURCE_DIR}/config/scene_config.json"
            "${CMAKE_CURRENT_BINARY_DIR}/config/scene_config.json"
        DEPENDS "config/scene_config.json"
)

add_custom_target(update_config ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/config/scene_config.json")
add_dependencies(graphics_coursework update_config)

add_dependencies(graphics_coursework copy-files rollercoaster-models)