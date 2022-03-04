#pragma once

#include "img/media_path.hpp"

#include <stdexcept>

const MediaPath one_lane_airport_sprite_path = { "airport_1lane.png" };
const MediaPath two_lane_airport_sprite_path = { "airport_2lane.png" };

// number of cycles needed to service an aircraft at a terminal
constexpr unsigned int SERVICE_CYCLES = 40u;
// speeds below the threshold speed loose altitude linearly
constexpr float SPEED_THRESHOLD = 0.05f;
// this models the speed with wich slow (speed < SPEED_THRESHOLD) aircrafts sink
constexpr float SINK_FACTOR = 0.1f;
// distances below this distance are considered equal (planes crash, waypoints
// are reached, etc)
constexpr float DISTANCE_THRESHOLD = 0.05f;
// each aircraft sprite has 8 tiles
constexpr unsigned char NUM_AIRCRAFT_TILES = 8;
// size of the plane-sprite on screen
constexpr float PLANE_TEXTURE_DIM = 0.2f;
// default number of ticks per second
constexpr unsigned int DEFAULT_TICKS_PER_SEC = 16u;
// default zoom factor
constexpr float DEFAULT_ZOOM = 2.0f;
// default window dimensions
constexpr size_t DEFAULT_WINDOW_WIDTH  = 800;
constexpr size_t DEFAULT_WINDOW_HEIGHT = 600;

using AircraftCrash = std::runtime_error;
