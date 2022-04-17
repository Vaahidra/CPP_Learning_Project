#pragma once

#include "geometry.hpp"
#include <stdexcept>
#include <ostream>

enum AircraftCrashReason {out_of_fuel, bad_landing};

class AircraftCrash : public std::runtime_error {
public:
    AircraftCrash(const std::string& flight_number, const AircraftCrashReason& reason)
        : std::runtime_error {build_error_msg(flight_number, reason)}
    {
        assert(!flight_number.empty());
    }
private:
    static std::string build_error_msg(const std::string& flight_number, const AircraftCrashReason& reason) {
        std::string msg;
        msg += flight_number + " has crashed into the ground because";
        msg += reason_to_string(reason) + ".\n";
        return msg;
    }
    static std::string reason_to_string(const AircraftCrashReason& reason) {
        if (reason == out_of_fuel) return " it's run out of fuel";
        if (reason == bad_landing) return " of a lack of landing skill";
        return " of nothing :suprised_face:";
    }
};
