#include "AircraftManager.hpp"
#include <utility>
#include <numeric>
#include <algorithm>
#include "aircraftCrash.hpp"


void AircraftManager::add(std::unique_ptr<Aircraft> aircraft)
{
    assert(aircraft);
    aircrafts.emplace_back(std::move(aircraft));
}



bool AircraftManager::move()
{   
    //sortAircrafts();
   
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
unsigned AircraftManager::get_required_fuel() {
    return std::accumulate(aircrafts.begin(), aircrafts.end(), 0,
           [](unsigned x, const std::unique_ptr<Aircraft>& a){
               return a->is_low_on_fuel() && a->is_circling() ? a->get_missing_fuel() + x : x;
           });
}
void AircraftManager::display_crash_number() const {
    std::cout << crash_count << " aircraft(s) have crashed.." << std::endl;
}

