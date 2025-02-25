#include "GameController.h"

//int tick = 0;

// Simply instantiates the game
GameController::GameController(
        const string &space_grid_file_name,
        const string &celestial_objects_file_name,
        const string &leaderboard_file_name,
        const string &player_file_name,
        const string &player_name

) {
    game = new AsteroidDash(space_grid_file_name, celestial_objects_file_name, leaderboard_file_name, player_file_name,
                            player_name);
    // TODO: Your code here, if you want to perform extra initializations
}

// Reads commands from the given input file, executes each command in a game tick
void GameController::play(const string &commands_file) {
    // TODO: Your code here

    std::vector<std::string> commands = read_commands(commands_file);
    game->current = game->celestial_objects_list_head;
    if (game->game_time == 0) {
        game->load_to_player_on_the_grid();
        game->print_space_grid();
    }
    
    for (int i = 0; i < static_cast<int>(commands.size()); i++) {
        if (commands[game->game_time] == "MOVE_UP") {
        game->player->move_up();
        game->update_space_grid();
        } else if (commands[game->game_time] == "MOVE_DOWN") {
            game->player->move_down(static_cast<int>(game->space_grid.size()) - 1);
            game->update_space_grid();
        } else if (commands[game->game_time] == "MOVE_RIGHT") {
            game->player->move_right(static_cast<int>(game->space_grid[0].size()) - 1);
            game->update_space_grid();
        } else if (commands[game->game_time] == "MOVE_LEFT") {
            game->player->move_left();
            game->update_space_grid();
        } else if (commands[game->game_time] == "SHOOT") {
            game->shoot();
            game->update_space_grid();
        } else if (commands[game->game_time] == "NOP") {
            game->update_space_grid();
        } else if (commands[game->game_time] == "PRINT_GRID") {
            game->print_space_grid();
        }
        game->print_space_grid();
    }
}

// Destructor to delete dynamically allocated member variables here
GameController::~GameController() {
    // TODO: Your code here
}

// Function to reads commands.
std::vector<std::string> GameController::read_commands(std::string commands_file) {
    std::string each_row;

    std::ifstream commands(commands_file);
    if (!commands.is_open()) {
        throw std::runtime_error("This file is not opened!"); 
    }
    std::vector<std::string> commands_vec;
    while (getline(commands, each_row)) {
        commands_vec.push_back(each_row);
    }
    return commands_vec;
}

