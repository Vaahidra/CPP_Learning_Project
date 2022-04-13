#pragma once

#include "geometry.hpp"

// runway positions relative to the airport position
struct Runway
{
    const Point3D start, end;

    Runway(const Point3D& start_, const float length = 1) :
        start { start_ }, end { start_ + Point3D { length, 0, 0 } }
    {}
};
