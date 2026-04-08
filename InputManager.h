//
// Created by aven on 08/04/2026.
//

#ifndef GRAPHICS_COURSEWORK_INPUTMANAGER_H
#define GRAPHICS_COURSEWORK_INPUTMANAGER_H
#include "Scene.h"


class InputManager {

    bool keys[512] = { false };
    Scene* scene;

public:

    InputManager(Scene* scene);

    //handle ASCII input
    void handle_input_down(unsigned char key, int x, int y);
    void handle_input_up(unsigned char key, int x, int y);

    //handle special character input
    void handle_input_down(int key, int x, int y);
    void handle_input_up(int key, int x, int y);

    void update() const;
};


#endif //GRAPHICS_COURSEWORK_INPUTMANAGER_H