#pragma once

#include "GL/dynamic_object.hpp"
#include "aircraft.hpp"

#include <memory>
#include <vector>

class AircraftManager : public GL::DynamicObject
{
private:
    std::vector<std::unique_ptr<Aircraft>> aircrafts;
public:

    void add(std::unique_ptr<Aircraft> aircraft);
    bool move() override;
    unsigned count_aircraft_on_airline(const std::string_view&);
};


