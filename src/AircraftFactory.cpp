#include "AircraftFactory.h"

#include "aircraft.hpp"

#include <fstream>

AircraftFactory::AircraftFactory()
{
    aircraft_types.emplace_back(std::make_unique<AircraftType>( .02f, .05f, .02f, .5f, 3'000, MediaPath { "l1011_48px.png" } ));
    aircraft_types.emplace_back(std::make_unique<AircraftType>( .02f, .05f, .02f, .2f, 2'500, MediaPath { "b707_jat.png"} ));
    aircraft_types.emplace_back(std::make_unique<AircraftType>( .02f, .1f, .02f, 1.f, 5'000, MediaPath { "concorde_af.png" } ));
    assert(aircraft_types.size() == 3);
}
std::unique_ptr<Aircraft> AircraftFactory::create_aircraft(Tower& tower)
{
    const std::string flight_number = new_flight_number();
    const float angle       = (std::rand() % 1000) * 2 * 3.141592f / 1000.f; // random angle between 0 and 2pi
    const Point3D start     = Point3D { std::sin(angle), std::cos(angle), 0.f } * 3 + Point3D { 0.f, 0.f, 2.f };
    const Point3D direction = (-start).normalize();
    const AircraftType& type = *aircraft_types[std::rand() % aircraft_types.size()];

    return std::make_unique<Aircraft>(type, flight_number, start, direction, tower);
}

std::string AircraftFactory::new_flight_number()
{
    std::string flight_number;
    do {
        flight_number = airlines[std::rand() % airlines.size()] + std::to_string(1000 + (rand() % 9000));
    } while (flight_numbers.find(flight_number) != flight_numbers.end());
    flight_numbers.emplace(flight_number);
    return flight_number;
}

std::unique_ptr<AircraftFactory> AircraftFactory::LoadTypes(const MediaPath& media)
{
    const std::string filePath = media.get_full_path().string();
    std::ifstream file (filePath);
    if (file.is_open()) {
        return std::make_unique<AircraftFactory>(file);
    }
    throw std::invalid_argument {"The given file cannot be opened!"};
}
AircraftFactory::AircraftFactory(std::ifstream& input)
{
    assert(input.is_open());
    std::string line;
    while (std::getline(input, line)) {
        aircraft_types.emplace_back(parse_line(line));
    }
}
std::unique_ptr<AircraftType> AircraftFactory::parse_line(std::string& line)
{
    size_t pos = 0;
    try {
        const float gSpeed = std::stof (line, &pos);
        line.erase(0, pos + 1);

        const float aSpeed = std::stof (line, &pos);
        line.erase(0, pos + 1);

        const float acc = std::stof (line, &pos);
        line.erase(0, pos + 1);

        const float consumption = std::stof (line, &pos);
        line.erase(0, pos + 1);

        const int fuel = std::stoi (line, &pos);
        line.erase(0, pos + 1);

        return std::make_unique<AircraftType>(gSpeed, aSpeed, acc, consumption, fuel, MediaPath {line});
    } catch (const std::invalid_argument& e) {
        throw std::invalid_argument{"File format invalid. The should be 'float float float float int string"};
    }
}
