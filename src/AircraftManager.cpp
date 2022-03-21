#include "AircraftManager.hpp"

#include <numeric>
#include <algorithm>

AircraftManager::AircraftManager()
{
    GL::move_queue.emplace(this);
}


bool AircraftManager::move_aircraft(double dt, Aircraft& craft) {
    assert(dt > 0);
    try {
        return craft.move(dt);
    } catch (const AircraftCrash& crash) {
        std::cerr << crash.what() << std::endl;
        crash_count++;
        return true;
    }
}
[[maybe_unused]] void AircraftManager::display_aircrafts() { // Debug function
    std::cout << "---" << std::endl;
    std::for_each(aircrafts.begin(), aircrafts.end(), [](const std::unique_ptr<Aircraft>& a){std::cout << *a << std::endl;});
    std::cout << "---" << std::endl;
}
void AircraftManager::move(const double dt)
{
    assert(dt > 0);
    std::sort(aircrafts.begin(), aircrafts.end(),
              [](const std::unique_ptr<Aircraft>& a, const std::unique_ptr<Aircraft>& b){return *a < *b;});
//    display_aircrafts();
    aircrafts.erase(std::remove_if(aircrafts.begin(), aircrafts.end(),
                  [this, dt](const std::unique_ptr<Aircraft>& a){return move_aircraft(dt, *a);}), aircrafts.end());
}
<<<<<<< HEAD

unsigned AircraftManager::count_aircraft_on_airline(const std::string_view& line)
{
    return std::count_if(aircrafts.begin(), aircrafts.end(),
        [line](const std::unique_ptr<Aircraft>& a){return (a->get_flight_num().rfind(line, 0) == 0);});
}

=======
>>>>>>> 0cd66b41b9545d4cb0625c02ef75b6aab2b2a223
void AircraftManager::add_aircraft(std::unique_ptr<Aircraft> aircraft)
{
    assert(aircraft != nullptr);
    aircrafts.emplace_back(std::move(aircraft));
}
