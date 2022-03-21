#pragma once

#include <ostream>
#include <fstream>
#include <vector>
#include <memory>

#include "aircraft_types.hpp"

class Aircraft;
class Tower;

inline std::array<std::string, 8> airlines { "AF", "LH", "EY", "DL", "KL", "BA", "AY", "EY" };

class AircraftFactory
{
public:
    AircraftFactory(const AircraftFactory&) = delete;               // Copy Constructor
    AircraftFactory& operator=(const AircraftFactory&) = delete;    // Assignment
    AircraftFactory();                                              // Base Constructor
    ~AircraftFactory() = default;                                   // Destructor
    // Fro private to public because cannot make unique_ptr with private constructor
    explicit AircraftFactory(std::ifstream&);
    static std::unique_ptr<AircraftFactory> LoadTypes(const MediaPath&);

    std::unique_ptr<Aircraft> create_aircraft(Tower& tower);
private:
    static std::unique_ptr<AircraftType> parse_line(std::string&);

    std::string new_flight_number();

    std::vector<std::unique_ptr<AircraftType>> aircraft_types;
    std::unordered_set<std::string> flight_numbers;
};
