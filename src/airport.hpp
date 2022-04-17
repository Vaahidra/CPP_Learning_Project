#pragma once

#include "GL/displayable.hpp"
#include "GL/dynamic_object.hpp"
#include "AircraftManager.hpp"
#include "GL/texture.hpp"
#include "airport_type.hpp"
#include "geometry.hpp"
#include "img/image.hpp"
#include "runway.hpp"
#include "terminal.hpp"
#include "tower.hpp"

#include <vector>

class Airport : public GL::Displayable, public GL::DynamicObject
{
private:
    const AirportType& type;
    const Point3D pos;
    const GL::Texture2D texture;
    std::vector<Terminal> terminals;
    AircraftManager& manager;
    Tower tower;
    unsigned fuel_stock = 0;
    unsigned ordered_fuel = 0;
    double next_refill_time = 0;
    // reserve a terminal
    // if a terminal is free, return
    // 1. a sequence of waypoints reaching the terminal from the runway-end and
    // 2. the number of the terminal (used for liberating the terminal later)
    // otherwise, return an empty waypoint-vector and any number
    std::pair<WaypointQueue, size_t> reserve_terminal(Aircraft& aircraft)
    {
        const auto it =
            std::find_if(terminals.begin(), terminals.end(), [](const Terminal& t) { return !t.in_use(); });

        if (it != terminals.end())
        {
            it->assign_craft(aircraft);
            const auto term_idx = std::distance(terminals.begin(), it);
            return { type.air_to_terminal(pos, 0, term_idx), term_idx };
        }
        else
        {
            return { {}, 0u };
        }
    }
    void refuel_all() {
        if (next_refill_time <= 0) {
            const auto old = ordered_fuel;
            fuel_stock += ordered_fuel;
            ordered_fuel = std::min(FUEL_TANKER, manager.get_required_fuel());
            next_refill_time = 100;
            std::cout << "Received : " << old << " | Stock : " << fuel_stock << " | Ordered : " << ordered_fuel << std::endl;
        } else {
            --next_refill_time;
        }
        std::for_each(terminals.begin(), terminals.end(), [this](Terminal& t){t.refill_aircraft_if_needed(fuel_stock);});
    }
    WaypointQueue start_path(const size_t terminal_number)
    {
        return type.terminal_to_air(pos, 0, terminal_number);
    }

    Terminal& get_terminal(const size_t terminal_num) { return terminals.at(terminal_num); }

public:
     ~Airport() override = default;
    Airport(const Airport&) = delete;
    Airport& operator=(const Airport&) = delete;
    Airport(const AirportType& type_, const Point3D& pos_, const img::Image* image, AircraftManager& _manager,
            const float z_ = 1.0f) :
        GL::Displayable { z_ },
        type { type_ },
        pos { pos_ },
        texture { image },
        terminals { type.create_terminals() },
        manager {_manager},
        tower { *this }
    {}

    Tower& get_tower() { return tower; }

    void display() const override { texture.draw(project_2D(pos), { 2.0f, 2.0f }); }

    bool move() override
    {
        for (auto& t : terminals)
        {
            t.move();
        }
        refuel_all();
        return true;
    }

    friend class Tower;
};
