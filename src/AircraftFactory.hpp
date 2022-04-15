#pragma once

#include "aircraft_types.hpp"
#include "AircraftManager.hpp"

class AircraftFactory
{
private:
    const std::string airlines[8] = { "AF", "LH", "EY", "DL", "KL", "BA", "AY", "EY" };
    std::vector<std::string> number_used;
    AircraftType* aircraft_types[3] {};


public:
// note: textures have to be initialized after initializing glut, so we'll need
// our own init here
    inline void init_aircraft_types()
    {
        aircraft_types[0] = new AircraftType { .02f, .05f, .02f, MediaPath { "l1011_48px.png" } };
        aircraft_types[1] = new AircraftType { .02f, .05f, .02f, MediaPath { "b707_jat.png" } };
        aircraft_types[2] = new AircraftType { .02f, .1f, .02f, MediaPath { "concorde_af.png" } };
    }
    void create_aircraft(const AircraftType& type, Tower& tower, AircraftManager& aircraft_manager)
    {
        std::string number = airlines[std::rand() % 8] + std::to_string(1000 + (rand() % 9000));
        auto it            = std::find(number_used.begin(), number_used.end(), number);

        while (it != number_used.end())
        {
            number = airlines[std::rand() % 8] + std::to_string(1000 + (rand() % 9000));
            it     = std::find(number_used.begin(), number_used.end(), number);
        }
        const float angle       = (rand() % 1000) * 2 * 3.141592f / 1000.f;
        const Point3D start     = Point3D { std::sin(angle), std::cos(angle), 0 } * 3 + Point3D { 0, 0, 2 };
        const Point3D direction = (-start).normalize();

        aircraft_manager.add( std::make_unique<Aircraft>(type, number, start, direction, tower));
        add_flight_number(number);
    }
    void create_random_aircraft(Tower& tower, AircraftManager& aircraft_manager)
    {
        create_aircraft(*(aircraft_types[rand() % 3]), tower, aircraft_manager);
    }
    void add_flight_number(const std::string num) { number_used.emplace_back(num); }
};