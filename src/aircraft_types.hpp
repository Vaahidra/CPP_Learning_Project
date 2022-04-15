#pragma once

#include "GL/texture.hpp"
#include "img/image.hpp"
#include "img/media_path.hpp"

#include <array>

struct AircraftType
{
    const float max_ground_speed;
    const float max_air_speed;
    const float max_accel;
    const GL::Texture2D texture;

    AircraftType(const float max_ground_speed_, const float max_air_speed_, const float max_accel_,
                 const MediaPath& sprite, const size_t num_tiles = NUM_AIRCRAFT_TILES) :
        max_ground_speed { max_ground_speed_ },
        max_air_speed { max_air_speed_ },
        max_accel { max_accel_ },
        texture { new img::Image { sprite.get_full_path() }, num_tiles }
    {}
};

