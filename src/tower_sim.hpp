#pragma once

class Airport;
struct AircraftType;

#include "airport.hpp"
#include "AircraftManager.hpp"

class TowerSimulation
{
private:
    bool help        = false;
    Airport* airport = nullptr;
    std::unique_ptr<AircraftManager> aircraft_manager;

    TowerSimulation(const TowerSimulation&) = delete;
    TowerSimulation& operator=(const TowerSimulation&) = delete;

    void create_aircraft(const AircraftType& type) const;
    void create_random_aircraft() const;

    void create_keystrokes() const;
    void display_help() const;

    void init_airport();

public:
    TowerSimulation(int argc, char** argv);
    ~TowerSimulation();

    void launch();
};
