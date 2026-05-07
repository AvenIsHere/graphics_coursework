//
// Created by aven on 30/04/2026.
//

#ifndef GRAPHICS_COURSEWORK_COASTERTRACK_H
#define GRAPHICS_COURSEWORK_COASTERTRACK_H

#include <vector>
#include <glm/glm.hpp>

#include "objects/ModelObject.h"


class CoasterTrack {
public:

    enum class TrackType {
        STRAIGHT,
        RIGHT,
        LEFT,
        LOOP,
        STEP_UP,
        STEP_DOWN,
    };

    struct TrackData {
        std::string piece_model;
        float rotation;
        glm::vec3 start_point;
        glm::vec3 end_point;
    };

    explicit CoasterTrack(const std::vector<TrackType> &givenTrack, glm::vec3 displacement);
    explicit CoasterTrack(glm::vec3 displacement);

    static void new_track(std::pair<TrackType, TrackData> given_track);
    static void new_tracks(const std::unordered_map<TrackType, TrackData>& given_tracks);

    void save_to_file() const;
    std::vector<std::shared_ptr<SceneObject>> load_from_file();

    static std::vector<TrackType> parse_txt_file(const std::string& file_path);

    std::shared_ptr<SceneObject> add_track(TrackType given_track);
    std::vector<std::shared_ptr<SceneObject>> add_tracks(const std::vector<TrackType>& given_tracks);

    std::shared_ptr<SceneObject> pop_track();
    std::vector<std::shared_ptr<SceneObject>> clear_tracks();

    [[nodiscard]] std::vector<std::shared_ptr<SceneObject>> get_model_objs();

private:

    static std::unordered_map<TrackType, TrackData> tracks;

    std::optional<glm::vec3> initial_start_point;
    glm::vec3 position{};
    glm::vec3 rotation{};

    std::vector<TrackType> track;
    std::vector<std::shared_ptr<SceneObject>> model_objects;

    [[nodiscard]] glm::vec3 rotation_vec(glm::vec3 direction) const;
    void handle_movement(TrackType type, bool undo);
};


#endif //GRAPHICS_COURSEWORK_COASTERTRACK_H
