#include "tower.hpp"

#include "airport.hpp"
#include "terminal.hpp"

#include <cassert>

WaypointQueue Tower::get_circle() const
{
    return { { Point3D { -1.5f, -1.5f, .5f }, wp_air },
             { Point3D { 1.5f, -1.5f, .5f }, wp_air },
             { Point3D { 1.5f, 1.5f, .5f }, wp_air },
             { Point3D { -1.5f, 1.5f, .5f }, wp_air } };
}

WaypointQueue Tower::get_instructions(Aircraft& aircraft)
{
    if (!aircraft.is_at_terminal)
    {
        // if the aircraft is far, then just guide it to the airport vicinity
        if (aircraft.distance_to(airport.pos) < 5)
        {
            // try and reserve a terminal for the craft to land
            const auto vp = airport.reserve_terminal(aircraft);
            if (!vp.first.empty())
            {
                reserved_terminals.emplace(&aircraft, vp.second);
                return vp.first;
            }
            else
            {
                return get_circle();
            }
        }
        else
        {
            return get_circle();
        }
    }
    else
    {
        // get a path for the craft to start
        const auto it = reserved_terminals.find(&aircraft);
        assert(it != reserved_terminals.end());
        const auto terminal_num = it->second;
        Terminal& terminal      = airport.get_terminal(terminal_num);
        if (!terminal.is_servicing()) // quitter le terminal
        {
            aircraft.is_service_done = true;
            terminal.finish_service(); // 
            reserved_terminals.erase(it);
            aircraft.is_at_terminal = false;
            return airport.start_path(terminal_num);
        }
        else
        {
            return {};
        }
    }
}

void Tower::arrived_at_terminal(const Aircraft& aircraft)
{
    const auto it = reserved_terminals.find(&aircraft);
    assert(it != reserved_terminals.end());
    airport.get_terminal(it->second).start_service(aircraft);
}


void Tower::ifReservedReleaseTerminal(Aircraft* aircraft)
{
    const auto iterator = reserved_terminals.find(aircraft); //Search for aircraft
    if (iterator == reserved_terminals.end())  //If it's the latest
        return;
    std::cout << "release crashed aircraft" << std::endl;
    const auto terminal_num = iterator->second; // Get terminal number that was reserved for crashed aircraft
    Terminal& terminal      = airport.get_terminal(terminal_num);
    terminal.releaseTerminal();
    reserved_terminals.erase(iterator);
}
// task2 | Objectif 2 | B.4
WaypointQueue Tower::reserve_terminal(Aircraft& aircraft)
{
    return aircraft.has_terminal() ? WaypointQueue {} : instruction_aux(aircraft);
}
WaypointQueue Tower::instruction_aux(Aircraft& aircraft) {
    if (aircraft.distance_to(airport.pos) >= 5) return {};            // If the aircraft is far -> cannot give him a terminal
    const auto vp = airport.reserve_terminal(aircraft);            // Try to reserve a terminal
    if (vp.first.empty()) return {};                                  // If no terminal left -> empty
    reserved_terminals.emplace(&aircraft, vp.second);                 // Otherwise -> reserve the terminal found
    return vp.first;                                                  // Return the path to the terminal
}