#ifndef ASTEROIDDASH_H
#define ASTEROIDDASH_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

#include "CelestialObject.h"
#include "LeaderboardEntry.h"
#include "Leaderboard.h"
#include "Player.h"

#define occupiedCellChar "██"
#define unoccupiedCellChar "▒▒"

using namespace std;

// Class that encapsulates the game play internals
class AsteroidDash {
public:

    // Constructor to initialize the game
    AsteroidDash(const string &space_grid_file_name, const string &celestial_objects_file_name,
                 const string &leaderboard_file_name, const string &player_file_name, const string &player_name);

    // Destructor. Remove dynamically allocated member variables here
    virtual ~AsteroidDash();

    // 2D space_grid
    vector<vector<int> > space_grid;

    // Pointer to track the player instance
    Player *player = nullptr;

    // A reference to the head of the celestial objects linked list
    CelestialObject *celestial_objects_list_head = nullptr;

    // Current score of the game
    unsigned long current_score = 0;

    // Current game tick
    unsigned long game_time = 0;

    // Leaderboard file name to store and read the leaderboard from
    string leaderboard_file_name;

    // Leaderboard member variable
    Leaderboard leaderboard;

    // True if the game is over
    bool game_over = false;

    // A reference to the current of the celestial objects linked list
    CelestialObject *current = nullptr;

    // A reference to the commands line file
    std::string commands_file;

    // The vector holding celestial objects.
    std::vector<CelestialObject*> celestial_vector;

    // Function to print the space_grid
    void print_space_grid() const;

    // Function to read the space grid from a file
    void read_space_grid(const string &input_file);

    // Function to read the player from a file
    void read_player(const string &player_file_name, const string &player_name);

    // Function to read celestial objects from a file
    // Reads the input file and calls the read_celestial_object() function for each celestial_object;
    void read_celestial_objects(const string &input_file);

    // Updates the grid based on player and celestial object states
    void update_space_grid();

    // Corresponds to the SHOOT command.
    void shoot();

    // Function to generates a Celestial object according to the given properties and adds a list.
    void generate_celestial_object(const std::vector<std::string> &celestial_object_attributes);

    // Function to check the similarity between the right rotation object and the original object.
    bool check_the_shapes(const std::vector<std::vector<bool>> &original_shape, const std::vector<std::vector<bool>> &copied_shape);

    // Function to loads the player on the grid.
    void load_to_player_on_the_grid();

    // Function to loads the celestial objects on the grid.
    void load_to_celestialobjects_on_the_grid();
    //void load_to_celestialobjects_on_the_grid(CelestialObject* celestial_object);

    // Function to cleans the grid.
    void clear_the_grid();

    // Function to controlling the collision of a spaceship with a celestial object.
    bool is_there_a_collision_with_a_projectile(int row, int col);

    /* // Function to reads commands.
    std::vector<std::string> read_commands(std::string commands_file); */

    // Function to update the celestial objects' locations.
    void update_celestial_locations();
    /* void update_celestial_locations(std::vector<CelestialObject*> para_celestial_vectors); */

    // Projectile struct.
    struct Projectile {
        int row_number;
        int col_number;
        bool type = true;
    };

    // Function to load the projectile.
    void load_the_projectile();

    // Function to update the locations of projectiles
    void update_projectile_locations();

    std::vector<Projectile> projectile_vector;

    void clear_the_celestial_objects_locations(CelestialObject* celestial_object);
};


#endif // ASTEROIDDASH_H
