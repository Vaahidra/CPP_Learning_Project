#pragma once

#include "waypoint.hpp"

#include <algorithm>
#include <unordered_map>
#include <utility>
#include <map>

class Airport;
class Aircraft;
class Terminal;

class Tower
{
private:
    //task_0 C-6
    using AircraftToTerminal = std::map<const Aircraft*, size_t>;


    Airport& airport;
    // aircrafts may reserve a terminal
    // if so, we need to save the terminal number in order to liberate it when the craft leaves
    AircraftToTerminal reserved_terminals = {};

    WaypointQueue get_circle() const;

    
public:
    Tower(Airport& airport_) : airport { airport_ } {}

    // produce instructions for aircraft
    WaypointQueue get_instructions(Aircraft& aircraft);
    void arrived_at_terminal(const Aircraft& aircraft);
    WaypointQueue reserve_terminal(Aircraft& aircraft);
    WaypointQueue instruction_aux(Aircraft& aircraft);
    void ifReservedReleaseTerminal(Aircraft* aircraft);
    void on_aircraft_crash(const Aircraft& aircraft);
};
