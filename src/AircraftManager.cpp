#include "AircraftManager.hpp"
#include <utility>



void AircraftManager::add(std::unique_ptr<Aircraft> aircraft)
{
    assert(aircraft);
    aircrafts.emplace_back(std::move(aircraft));
}

bool AircraftManager::move()
{
    aircrafts.erase(std::remove_if(aircrafts.begin(), aircrafts.end(),
                                   [](std::unique_ptr<Aircraft>& aircft)
                                   {
                                       try
                                       {
                                           return !aircft->move();
                                       } catch (const AircraftCrash& crash)
                                       {
                                           std::cerr << crash.what() << std::endl;
                                           return true;
                                       }
                                   }),
                    aircrafts.end());

    return true;
}
unsigned AircraftManager::count_aircraft_on_airline(const std::string_view& line)
{
    return std::count_if(aircrafts.begin(), aircrafts.end(),
        [line](const std::unique_ptr<Aircraft>& a){return (a->get_flight_num().rfind(line, 0) == 0);});
}


