//
// Created by aven on 30/04/2026.
//

#include "CoasterTrack.h"

#include <utility>
#include <glm/ext/matrix_transform.hpp>

std::unordered_map<CoasterTrack::TrackType, CoasterTrack::TrackData> CoasterTrack::tracks;

CoasterTrack::CoasterTrack(std::vector<TrackType> givenTrack, const glm::vec3 displacement) {
    track = std::move(givenTrack);
    if (!track.empty()) {
        initial_start_point = tracks.at(track.at(0)).start_point + displacement;
    }
    else {
        initial_start_point = std::nullopt;
    }
    position = displacement;
}

CoasterTrack::CoasterTrack(const glm::vec3 displacement) {
    track = {};
    initial_start_point = std::nullopt;
    position = displacement;
}

void CoasterTrack::new_track(std::pair<TrackType, TrackData> given_track) {
    tracks.emplace(given_track);
}

void CoasterTrack::new_tracks(const std::unordered_map<TrackType, TrackData>& given_tracks) {
    for (const auto& track : given_tracks) {
        new_track(track);
    }
}

std::shared_ptr<SceneObject> CoasterTrack::add_track(TrackType given_track) {
    track.emplace_back(given_track);
    const auto& data = tracks.at(given_track);
    const glm::vec3 obj_origin = position - rotation_vec(data.start_point);

    const auto object = model_objects.emplace_back(std::make_shared<ModelObject>(data.piece_model, obj_origin, glm::vec3(1,1,1)));

    object->rotate(rotation.x, {1,0,0});
    object->rotate(rotation.y, {0,1,0});
    object->rotate(rotation.z, {0,0,1});

    handle_movement(given_track, false);
    if (!initial_start_point) {
        initial_start_point = tracks.at(given_track).start_point + position;
    }
    return object;
}

std::vector<std::shared_ptr<SceneObject>> CoasterTrack::add_tracks(const std::vector<TrackType>& given_tracks) {
    std::vector<std::shared_ptr<SceneObject>> return_objects;
    for (const auto& given_track : given_tracks) {
        return_objects.emplace_back(add_track(given_track));
    }
    return return_objects;
}

std::optional<CoasterTrack::TrackType> CoasterTrack::pop_track() {
    std::optional<TrackType> return_track;
    if (!track.empty()) {
        return_track = track.back();
        track.pop_back();
    }
    return return_track;
}

glm::vec3 CoasterTrack::rotation_vec(const glm::vec3 direction) const {
    glm::mat4 rotate(1.0f);
    rotate = glm::rotate(rotate, rotation.y, glm::vec3(0,1,0));
    rotate = glm::rotate(rotate, rotation.x, glm::vec3(1,0,0));
    rotate = glm::rotate(rotate, rotation.z, glm::vec3(0,0,1));
    return {rotate * glm::vec4(direction, 0.0f)};
}

void CoasterTrack::handle_movement(const TrackType type, bool undo) {
    const auto& data = tracks.at(type);
    position += rotation_vec(data.end_point - data.start_point);
    rotation.y += data.rotation;
}

std::vector<std::shared_ptr<SceneObject>> CoasterTrack::get_model_objs() {

    for (const auto track_piece : track) {
        const auto& data = tracks.at(track_piece);
        const glm::vec3 obj_origin = position - rotation_vec(data.start_point);

        const auto object = model_objects.emplace_back(std::make_shared<ModelObject>(data.piece_model, obj_origin, glm::vec3(1,1,1)));

        object->rotate(rotation.x, {1,0,0});
        object->rotate(rotation.y, {0,1,0});
        object->rotate(rotation.z, {0,0,1});

        handle_movement(track_piece, false);
    }

    return model_objects;
}
