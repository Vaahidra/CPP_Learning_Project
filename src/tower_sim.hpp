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
<<<<<<< HEAD
    std::unique_ptr<AircraftFactory> aircraft_factory;
=======
>>>>>>> 0cd66b41b9545d4cb0625c02ef75b6aab2b2a223

    std::string data_path;
    void create_aircraft(const AircraftType& type) const;
    void create_random_aircraft() const;

    void create_keystrokes() const;
    void display_help() const;

    void init_airport();

public:
    TowerSimulation(int argc, char** argv);
    ~TowerSimulation();
    TowerSimulation(const TowerSimulation&) = delete;
    TowerSimulation& operator=(const TowerSimulation&) = delete;
    void launch();
};
