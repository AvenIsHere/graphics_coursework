//
// Created by aven on 08/04/2026.
//

#ifndef GRAPHICS_COURSEWORK_INPUTMANAGER_H
#define GRAPHICS_COURSEWORK_INPUTMANAGER_H
#include <functional>
#include <map>


struct InputMapping {
    int key;
    std::function<void()> function;
    bool special = false;
};

class InputManager {

    static inline bool keys[512] = { false };
    static std::map<int, std::function<void()>> functions;

public:

    //handle ASCII input
    static void handle_input_down(unsigned char key, int x, int y);
    static void handle_input_up(unsigned char key, int x, int y);

    //handle special character input
    static void handle_input_down(int key, int x, int y);
    static void handle_input_up(int key, int x, int y);

    static void update();

    static void add_to_map(int key, std::function<void()> func, bool special=false);
    static void add_mappings(const std::vector<InputMapping>& mappings);
};


#endif //GRAPHICS_COURSEWORK_INPUTMANAGER_H