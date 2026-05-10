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
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include "nfd.hpp"

std::unordered_map<std::string, CoasterTrack::TrackData> CoasterTrack::tracks;

CoasterTrack::CoasterTrack(const std::vector<std::string> &given_track, const glm::vec3 displacement) {
    position = displacement;
    add_tracks(given_track);
}

CoasterTrack::CoasterTrack(const std::string &json_path, glm::vec3 displacement) {
    position = displacement;
    add_tracks(parse_json(json_path));
}

CoasterTrack::CoasterTrack(const glm::vec3 displacement) {
    track = {};
    initial_start_point = std::nullopt;
    position = displacement;
}

void CoasterTrack::new_track(std::pair<std::string, TrackData> given_track) {
    tracks.emplace(given_track);
}

void CoasterTrack::new_tracks(const std::unordered_map<std::string, TrackData>& given_tracks) {
    for (const auto& track : given_tracks) {
        new_track(track);
    }
}

std::shared_ptr<SceneObject> CoasterTrack::add_track(std::string given_track) {
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

std::vector<std::shared_ptr<SceneObject>> CoasterTrack::add_tracks(const std::vector<std::string>& given_tracks) {
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

void CoasterTrack::handle_movement(const std::string type, bool undo) {
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
    std::strftime(buf, sizeof(buf), "coaster_%Y-%m-%d_%H-%M-%S.json", time_now);
    std::ofstream save_file(buf);

    json json_tracks = json::array();
    for (const auto& given_track : track) {
        json_tracks.push_back(given_track);
    }

    json result = {"tracks", json_tracks};
    save_file << result;
}

std::vector<std::shared_ptr<SceneObject>> CoasterTrack::load_from_file() {
    clear_tracks();

    NFD_Init();
    NFD::UniquePath file_path;
    constexpr nfdu8filteritem_t filters[1] = {{"JSON file", "json"}};
    const nfdresult_t result = NFD::OpenDialog(file_path, filters, 1);
    if (result == NFD_CANCEL) {
        NFD_Quit();
        return {};
    }
    if (result != NFD_OKAY) throw std::runtime_error("Error opening file");
    NFD_Quit();

    const auto tracks_to_add = parse_json(file_path.get());
    return add_tracks(tracks_to_add);
}

std::vector<std::string> CoasterTrack::parse_json(const std::string& file_path) {

    std::ifstream coaster_file(file_path);
    auto json = json::parse(coaster_file);
    auto tracks_to_add = json["tracks"];

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
