#pragma once

#include "airport.hpp"
#include "AircraftManager.hpp"
#include "AircraftFactory.hpp"
class Airport;
struct AircraftType;

class TowerSimulation
{
private:
    bool help        = false;
    Airport* airport = nullptr;
    AircraftManager aircraft_manager;
    AircraftFactory factory;
    TowerSimulation(const TowerSimulation&) = delete;
    TowerSimulation& operator=(const TowerSimulation&) = delete;


    void create_keystrokes();
    void display_help() const;
    void display_airline(unsigned);
    void init_airport();

public:
    TowerSimulation(int argc, char** argv);
    ~TowerSimulation();

    void launch();
};
