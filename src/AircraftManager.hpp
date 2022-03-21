#pragma once

#include <ostream>
#include <vector>
#include <memory>
#include "aircraft.hpp"
#include "GL/dynamic_object.hpp"

class Aircraft;

class AircraftManager : public GL::DynamicObject
{
public:
    AircraftManager();                       // Base Constructor
    ~AircraftManager() override = default;   // Destructor
    AircraftManager(const AircraftManager&) = delete;
    AircraftManager& operator=(const AircraftManager&) = delete;
    unsigned count_aircraft_on_airline(const std::string_view&);
    void add_aircraft(std::unique_ptr<Aircraft>);
    void move(double) override;

private:
    unsigned crash_count = 0;
    std::vector<std::unique_ptr<Aircraft>> aircrafts;
    bool move_aircraft(double dt, Aircraft &craft);
};