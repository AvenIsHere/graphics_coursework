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
    };

    struct TrackData {
        std::string piece_model;
        glm::vec3 displacement;
        float rotation;
    };

    explicit CoasterTrack(std::vector<TrackType> givenTrack);

    static void add_track(std::pair<TrackType, TrackData> given_track);
    static void add_tracks(const std::unordered_map<TrackType, TrackData>& given_tracks);

    [[nodiscard]] std::vector<std::shared_ptr<SceneObject>> get_model_objs();

private:

    static std::unordered_map<TrackType, TrackData> tracks;

    [[nodiscard]] glm::vec3 rotation_vec(glm::vec3 direction) const;
    void handle_movement(TrackType type, const std::shared_ptr<SceneObject> &);

    std::vector<TrackType> track;
    glm::vec3 position{};
    glm::vec3 rotation{};

    static constexpr float STANDARD_WIDTH = 2.73985;
};


#endif //GRAPHICS_COURSEWORK_COASTERTRACK_H
