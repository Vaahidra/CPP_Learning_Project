#include "aircraft.hpp"

#include "GL/opengl_interface.hpp"
#include "aircraftCrash.hpp"
#include <cmath>


void Aircraft::turn_to_waypoint()
{
    if (!waypoints.empty())
    {
        Point3D target = waypoints[0];
        if (waypoints.size() > 1)
        {
            const float d   = (waypoints[0] - pos).length();
            const Point3D W = (waypoints[0] - waypoints[1]).normalize(d / 2.0f);
            target += W;
        }

        turn(target - pos - speed);
    }
}

void Aircraft::turn(Point3D direction)
{
    (speed += direction.cap_length(type.max_accel)).cap_length(max_speed());
}
void Aircraft::refill(unsigned int& fuel_stock) {
    const auto needed = get_missing_fuel();
    if (fuel_stock == 0) return;
    if (fuel_stock < needed) {
//        std::cout << "Refuelling " << fuel_stock << ". Aircraft not full." << std::endl;
        fuel += fuel_stock;
        fuel_stock = 0;
    } else {
//        std::cout << "Refuelling " << needed << ". Aircraft full." << std::endl;
        fuel += needed;
        fuel_stock -= needed;
    }
}
unsigned int Aircraft::get_speed_octant() const
{
    const float speed_len = speed.length();
    if (speed_len > 0)
    {
        const Point3D norm_speed { speed * (1.0f / speed_len) };
        const float angle =
            (norm_speed.y() > 0) ? 2.0f * 3.141592f - std::acos(norm_speed.x()) : std::acos(norm_speed.x());
        // partition into NUM_AIRCRAFT_TILES equal pieces
        return (static_cast<int>(std::round((angle * NUM_AIRCRAFT_TILES) / (2.0f * 3.141592f))) + 1) %
               NUM_AIRCRAFT_TILES;
    }
    else
    {
        return 0;
    }
}

// when we arrive at a terminal, signal the tower
void Aircraft::arrive_at_terminal()
{
    // we arrived at a terminal, so start servicing
    control.arrived_at_terminal(*this);
    is_at_terminal = true;
}

// deploy and retract landing gear depending on next waypoints
void Aircraft::operate_landing_gear()
{
    if (waypoints.size() > 1u)
    {
        const auto it            = waypoints.begin();
        const bool ground_before = it->is_on_ground();
        const bool ground_after  = std::next(it)->is_on_ground();
        // deploy/retract landing gear when landing/lifting-off
        if (ground_before && !ground_after)
        {
            std::cout << flight_number << " lift off" << std::endl;
        }
        else if (!ground_before && ground_after)
        {
            std::cout << flight_number << " is now landing..." << std::endl;
            landing_gear_deployed = true;
        }
        else if (!ground_before && !ground_after)
        {
            landing_gear_deployed = false;
        }
    }
}

void Aircraft::add_waypoint(const Waypoint& wp, const bool front)
{
    if (front)
    {
        waypoints.push_front(wp);
    }
    else
    {
        waypoints.push_back(wp);
    }
}

bool Aircraft::move()
{
    if (waypoints.empty())
    {
         if (is_service_done)
        {
            return false;
        }
        waypoints = control.get_instructions(*this);
    }

    if (fuel <= 0) {
        
        if (this->has_terminal())
            this->releaseTerminal();

        //crash_animation(MediaPath { "explostion3.png" }); // N'affiche rien :(, je ne sais pas pourquoi
        throw AircraftCrash {flight_number, out_of_fuel};
        //throw AircraftCrash { flight_number + "CRASHED ::: Out of Fuel" };
    
    }
    if (!is_at_terminal)
    {
        turn_to_waypoint();
        // move in the direction of the current speed
        pos += speed;

        // if we are close to our next waypoint, stike if off the list
        if (!waypoints.empty() && distance_to(waypoints.front()) < DISTANCE_THRESHOLD)
        {
            if (waypoints.front().is_at_terminal())
            {
                arrive_at_terminal();
            }
            else
            {
                operate_landing_gear();
            }
            waypoints.pop_front();
        }

        if (is_on_ground())
        {
            if (!landing_gear_deployed)
            {
                using namespace std::string_literals;
                throw AircraftCrash {flight_number, bad_landing};
                //throw AircraftCrash { flight_number + "bad landing" };

            }
        }
        else
        {
            // if we are in the air, but too slow, then we will sink!
            const float speed_len = speed.length();
             fuel = fuel - 1;
            if (speed_len < SPEED_THRESHOLD)
            {
                pos.z() -= SINK_FACTOR * (SPEED_THRESHOLD - speed_len);
            }
        }

        // update the z-value of the displayable structure
        GL::Displayable::z = pos.x() + pos.y();
    }
    return true;
}

//task2 | Objectif 2 | B.1
bool Aircraft::has_terminal () const
{
    return !waypoints.empty() && waypoints.back().is_at_terminal();
}
//task2 | Objectif 2 | B.2
bool Aircraft::is_circling() const
{
    return !is_service_done && !is_on_ground() && !has_terminal();
}
// task2 | Objectif 2 | B.4
void Aircraft::releaseTerminal()
{
    control.ifReservedReleaseTerminal(this);
}
void Aircraft::crash_animation(const MediaPath& sprite) const
{
    GL::Texture2D texture = new img::Image { sprite.get_full_path()};
    texture.draw(project_2D(pos), { PLANE_TEXTURE_DIM , PLANE_TEXTURE_DIM }, get_speed_octant());
}
void Aircraft::display() const
{
    type.texture.draw(project_2D(pos), { PLANE_TEXTURE_DIM, PLANE_TEXTURE_DIM }, get_speed_octant());
}
//Task 2 | Objectif 2 D-1
bool Aircraft::is_low_on_fuel() const
{
    return fuel < NEEDED_FUEL;
}
