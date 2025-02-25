#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "AsteroidDash.h"
#include "Player.h"
#include "CelestialObject.h"
#include "Leaderboard.h"
#include <vector>
#include <string>
#include <chrono>
#include <thread>


class GameController {
public:

    // Game instance
    AsteroidDash *game;

    // Tick
    int tick;

    // Constructor
    GameController(
            const string &space_grid_file_name,
            const string &celestial_objects_file_name,
            const string &leaderboard_file_name,
            const string &player_file_name,
            const string &player_name
    );

    // Reads commands from the given input file, executes each command in a tick
    void play(const string &commands_file);

    // Destructor to delete dynamically allocated member variables here
    virtual ~GameController();

    // Function to reads commands.
    std::vector<std::string> read_commands(std::string commands_file);
};

#endif // GAMECONTROLLER_H
