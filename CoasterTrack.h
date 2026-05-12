//
// Created by aven on 30/04/2026.
//

#ifndef GRAPHICS_COURSEWORK_COASTERTRACK_H
#define GRAPHICS_COURSEWORK_COASTERTRACK_H

#include <vector>
#include <glm/glm.hpp>

#include "objects/Cart.h"
#include "objects/ModelObject.h"


class CoasterTrack {
public:

    struct TrackData {
        std::string piece_model;
        float rotation;
        glm::vec3 start_point;
        glm::vec3 end_point;
        std::vector<glm::vec3> points;
    };

    explicit CoasterTrack(const std::vector<std::string> &given_track, glm::vec3 displacement, const std::string& cart_model);
    explicit CoasterTrack(const std::string &json_path, glm::vec3 displacement, const std::string& cart_model);
    explicit CoasterTrack(glm::vec3 displacement, const std::string &cart_model);

    static void new_track(std::pair<std::string, TrackData> given_track);
    static void new_tracks(const std::unordered_map<std::string, TrackData>& given_tracks);

    void save_to_file() const;
    std::vector<std::shared_ptr<SceneObject>> load_from_file();

    static std::vector<std::string> parse_json(const std::string& file_path);

    std::vector<std::shared_ptr<SceneObject>> add_track(const std::string& given_track);
    std::vector<std::shared_ptr<SceneObject>> add_tracks(const std::vector<std::string>& given_tracks);

    std::vector<std::shared_ptr<SceneObject>> pop_track();
    std::vector<std::shared_ptr<SceneObject>> clear_tracks();

    [[nodiscard]] glm::vec3 get_cart_location() const;
    [[nodiscard]] glm::vec3 get_cart_size() const;

    void start_cart(float speed);
    void stop_cart();
    void set_cart_speed(float speed);

    void change_cart_speed(float change);

    void update(int time_elapsed, int prev_time_elapsed);

    [[nodiscard]] std::vector<std::shared_ptr<SceneObject>> get_model_objs();

private:

    static std::unordered_map<std::string, TrackData> tracks;

    std::optional<glm::vec3> initial_start_point;
    std::string coaster_model;
    glm::vec3 position{};
    glm::vec3 rotation{};

    int current_segment = 0;
    float segment_progress = 0.0f;
    float cart_speed = 0.01f;
    bool is_moving = false;

    std::vector<std::string> track;
    std::vector<std::shared_ptr<SceneObject>> model_objects;
    std::vector<glm::vec3> segment_positions;
    std::vector<float> segment_y_rotations;

    std::shared_ptr<Cart> cart;

    [[nodiscard]] glm::vec3 rotation_vec(glm::vec3 direction) const;
    void handle_movement(const std::string& type, bool undo);
};


#endif //GRAPHICS_COURSEWORK_COASTERTRACK_H
