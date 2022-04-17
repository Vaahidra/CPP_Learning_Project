#pragma once

#include "GL/dynamic_object.hpp"
#include "aircraft.hpp"

#include <memory>
#include <vector>

class AircraftManager : public GL::DynamicObject
{
private:
    std::vector<std::unique_ptr<Aircraft>> aircrafts;
    unsigned crash_count = 0;

public:

    void add(std::unique_ptr<Aircraft> aircraft);
    bool move() override;
    unsigned count_aircraft_on_airline(const std::string_view&);
    unsigned get_required_fuel();
    void sortAircrafts()
    {
        std::sort(aircrafts.begin(), aircrafts.end(),
                  [](std::unique_ptr<Aircraft>& a, std::unique_ptr<Aircraft>& b)
                  {
                      if (a->has_terminal() == b->has_terminal())
                          return a->getFuel() <= b->getFuel();
                      return a->has_terminal();
                  });
    }

    void display_crash_number() const;
};
