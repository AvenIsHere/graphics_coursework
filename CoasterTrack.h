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

    struct TrackData {
        std::string piece_model;
        float rotation;
        glm::vec3 start_point;
        glm::vec3 end_point;
    };

    explicit CoasterTrack(const std::vector<std::string> &given_track, glm::vec3 displacement);
    explicit CoasterTrack(const std::string &json_path, glm::vec3 displacement);
    explicit CoasterTrack(glm::vec3 displacement);

    static void new_track(std::pair<std::string, TrackData> given_track);
    static void new_tracks(const std::unordered_map<std::string, TrackData>& given_tracks);

    void save_to_file() const;
    std::vector<std::shared_ptr<SceneObject>> load_from_file();

    static std::vector<std::string> parse_json(const std::string& file_path);

    std::shared_ptr<SceneObject> add_track(std::string given_track);
    std::vector<std::shared_ptr<SceneObject>> add_tracks(const std::vector<std::string>& given_tracks);

    std::shared_ptr<SceneObject> pop_track();
    std::vector<std::shared_ptr<SceneObject>> clear_tracks();

    [[nodiscard]] std::vector<std::shared_ptr<SceneObject>> get_model_objs();

private:

    static std::unordered_map<std::string, TrackData> tracks;

    std::optional<glm::vec3> initial_start_point;
    glm::vec3 position{};
    glm::vec3 rotation{};

    std::vector<std::string> track;
    std::vector<std::shared_ptr<SceneObject>> model_objects;

    [[nodiscard]] glm::vec3 rotation_vec(glm::vec3 direction) const;
    void handle_movement(std::string type, bool undo);
};


#endif //GRAPHICS_COURSEWORK_COASTERTRACK_H
