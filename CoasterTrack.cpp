//
// Created by aven on 30/04/2026.
//

#include "CoasterTrack.h"

#include <utility>
#include <glm/ext/matrix_transform.hpp>

std::unordered_map<CoasterTrack::TrackType, CoasterTrack::TrackData> CoasterTrack::tracks;

CoasterTrack::CoasterTrack(std::vector<TrackType> givenTrack) {
    track = std::move(givenTrack);
}

void CoasterTrack::add_track(std::pair<TrackType, TrackData> given_track) {
    tracks.emplace(given_track);
}

void CoasterTrack::add_tracks(const std::unordered_map<TrackType, TrackData>& given_tracks) {
    for (const auto& track : given_tracks) {
        add_track(track);
    }
}

glm::vec3 CoasterTrack::rotation_vec(const glm::vec3 direction) const {
    glm::mat4 rotate(1.0f);
    rotate = glm::rotate(rotate, rotation.x, glm::vec3(1,0,0));
    rotate = glm::rotate(rotate, rotation.y, glm::vec3(0,1,0));
    rotate = glm::rotate(rotate, rotation.z, glm::vec3(0,0,1));
    return {rotate * glm::vec4(direction, 0.0f)};
}

void CoasterTrack::handle_movement(const TrackType type, const std::shared_ptr<SceneObject> &object) {
    const auto displacement = object->get_aabb_dimensions();
    position += displacement.x * rotation_vec(glm::vec3(1, 0, 0));
    // position += displacement.y * rotation_vec(glm::vec3(0, 1, 0));
    position += displacement.z * rotation_vec(glm::vec3(0, 0, 1));
    position.z -= STANDARD_WIDTH;
    rotation.y += tracks.at(type).rotation;
}

std::vector<std::shared_ptr<SceneObject>> CoasterTrack::get_model_objs() {
    std::vector<std::shared_ptr<SceneObject>> model_objects;

    for (const auto track_piece : track) {
        auto object = model_objects.emplace_back(std::make_shared<ModelObject>(tracks.at(track_piece).piece_model, position, glm::vec3(1,1,1)));
        object->rotate(rotation.x, {1,0,0});
        object->rotate(rotation.y, {0,1,0});
        object->rotate(rotation.z, {0,0,1});
        const glm::vec3 corner_to_center = object->get_aabb_dimensions() * 0.5f;
        object->move(rotation_vec(corner_to_center));
        handle_movement(track_piece, object);
    }

    return model_objects;
}
