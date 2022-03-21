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
<<<<<<< HEAD
    unsigned count_aircraft_on_airline(const std::string_view&);
=======
    
>>>>>>> 0cd66b41b9545d4cb0625c02ef75b6aab2b2a223
    void add_aircraft(std::unique_ptr<Aircraft>);
    void move(double) override;

private:
<<<<<<< HEAD
    unsigned crash_count = 0;
=======
>>>>>>> 0cd66b41b9545d4cb0625c02ef75b6aab2b2a223
    std::vector<std::unique_ptr<Aircraft>> aircrafts;
    bool move_aircraft(double dt, Aircraft &craft);
};