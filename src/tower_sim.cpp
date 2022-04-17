#include "tower_sim.hpp"

#include "GL/opengl_interface.hpp"

#include "airport.hpp"
#include "config.hpp"
#include "img/image.hpp"
#include "img/media_path.hpp"

#include <algorithm>

#include <cstdlib>
#include <ctime>

using namespace std::string_literals;


TowerSimulation::TowerSimulation(int argc, char** argv) :
    help { (argc > 1) && (std::string { argv[1] } == "--help"s || std::string { argv[1] } == "-h"s) }
{
    MediaPath::initialize(argv[0]);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    GL::init_gl(argc, argv, "Airport Tower Simulation");

    create_keystrokes();
    GL::move_queue.emplace(&aircraft_manager);
}

TowerSimulation::~TowerSimulation()
{
    delete airport;
}
void TowerSimulation::display_airline(unsigned number) {
    assert(number <= 7);
    const std::string& airline = airlines[number];
    const unsigned count = aircraft_manager.count_aircraft_on_airline(airline);
    std::cout << count << " aircraft for the line : " << airline << std::endl;
}
void TowerSimulation::create_keystrokes() 
{
    GL::keystrokes.emplace('x', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('q', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('c', [this]() {factory.create_random_aircraft(airport->get_tower(), aircraft_manager);});
    GL::keystrokes.emplace('+', []() { GL::change_zoom(0.95f); });
    GL::keystrokes.emplace('-', []() { GL::change_zoom(1.05f); });
    GL::keystrokes.emplace('f', []() { GL::toggle_fullscreen(); });
    GL::keystrokes.emplace('a', []() { GL::increaseTick(); });
    GL::keystrokes.emplace('z', []() { GL::decreaseTick(); });
    GL::keystrokes.emplace('p', []() { GL::pause(); });
    GL::keystrokes.emplace('m', [this]() { aircraft_manager.display_crash_number(); });
    for (auto i = 0; i < 8; i++) {
        GL::keystrokes.emplace('0'+i, [this, i]() { display_airline(i); });
    }
}

void TowerSimulation::display_help() const
{
    std::cout << "This is an airport tower simulator" << std::endl
              << "the following keysstrokes have meaning:" << std::endl;

    for (const auto& ks_pair : GL::keystrokes)
    {
        std::cout << ks_pair.first << ' ';
    }

    std::cout << std::endl;
}

void TowerSimulation::init_airport()
{
    airport =
        new Airport { one_lane_airport, Point3D { 0, 0, 0 },
                      new img::Image { one_lane_airport_sprite_path.get_full_path() }, aircraft_manager };

    GL::move_queue.emplace(airport);
}

void TowerSimulation::launch()
{
    if (help)
    {
        display_help();
        return;
    }

    init_airport();
    factory.init_aircraft_types();
    GL::loop();
}
