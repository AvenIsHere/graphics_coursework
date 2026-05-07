//
// Created by aven on 30/04/2026.
//

#include "CoasterTrack.h"

#include <utility>
#include <glm/ext/matrix_transform.hpp>
#include <fstream>
#include <chrono>
#include <magic_enum/magic_enum.hpp>
#include <nfd.h>
#include <regex>

#include "nfd.hpp"

std::unordered_map<CoasterTrack::TrackType, CoasterTrack::TrackData> CoasterTrack::tracks;

CoasterTrack::CoasterTrack(const std::vector<TrackType> &givenTrack, const glm::vec3 displacement) {
    position = displacement;
    add_tracks(givenTrack);
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

std::shared_ptr<SceneObject> CoasterTrack::pop_track() {
    std::shared_ptr<SceneObject> return_track;
    if (!track.empty()) {
        const auto track_type = track.back();
        track.pop_back();
        handle_movement(track_type, true);
        return_track = model_objects.back();
        model_objects.pop_back();
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
    if (!undo) {
        position += rotation_vec(data.end_point - data.start_point);
        rotation.y += data.rotation;
        return;
    }
    rotation.y -= data.rotation;
    position -= rotation_vec(data.end_point - data.start_point);
}

void CoasterTrack::save_to_file() const {
    const auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    const auto time_now = std::localtime(&time);
    char buf[64];
    std::strftime(buf, sizeof(buf), "coaster_%Y-%m-%d_%H-%M-%S.txt", time_now);
    std::ofstream save_file(buf);
    for (const auto& given_track : track) {
        save_file << "CoasterTrack::TrackType::" << magic_enum::enum_name(given_track) << "," << std::endl;
    }
}

std::vector<std::shared_ptr<SceneObject>> CoasterTrack::load_from_file() {
    clear_tracks();

    NFD_Init();
    NFD::UniquePath file_path;
    constexpr nfdu8filteritem_t filters[1] = {{"Text file", "txt"}};
    const nfdresult_t result = NFD::OpenDialog(file_path, filters, 1);
    if (result == NFD_CANCEL) {
        NFD_Quit();
        return {};
    }
    if (result != NFD_OKAY) throw std::runtime_error("Error opening file");
    NFD_Quit();

    const auto tracks_to_add = parse_txt_file(file_path.get());
    return add_tracks(tracks_to_add);
}

std::vector<CoasterTrack::TrackType> CoasterTrack::parse_txt_file(const std::string& file_path) {

    std::ifstream coaster_file(file_path);
    std::stringstream buffer;
    buffer << coaster_file.rdbuf();
    std::string coaster_string = buffer.str();

    std::vector<TrackType> tracks_to_add;

    std::regex pattern("CoasterTrack::TrackType::(\\w+),");
    std::smatch matches;

    auto pos = coaster_string.find('\n');
    while (pos != std::string::npos) {
        if (std::string cur_track = coaster_string.substr(0, pos); std::regex_search(cur_track, matches, pattern)) {
            if (auto type = magic_enum::enum_cast<TrackType>(matches[1].str()); type.has_value()) {
                tracks_to_add.push_back(type.value());
            }
        }
        coaster_string.erase(0, pos + 1);
        pos = coaster_string.find('\n');
    }

    return tracks_to_add;
}

std::vector<std::shared_ptr<SceneObject> > CoasterTrack::clear_tracks() {
    std::vector<std::shared_ptr<SceneObject>> return_tracks;
    std::shared_ptr<SceneObject> obj = pop_track();
    while (obj) {
        return_tracks.push_back(obj);
        obj = pop_track();
    }
    initial_start_point = std::nullopt;
    return return_tracks;
}

std::vector<std::shared_ptr<SceneObject> > CoasterTrack::get_model_objs() {
    return model_objects;
}
