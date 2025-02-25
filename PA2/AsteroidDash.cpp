#include "AsteroidDash.h"
//#include "GameController.h"

//extern int tick;

// Constructor to initialize AsteroidDash with the given parameters
AsteroidDash::AsteroidDash(const string &space_grid_file_name,
                           const string &celestial_objects_file_name,
                           const string &leaderboard_file_name,
                           const string &player_file_name,
                           const string &player_name)

        : leaderboard_file_name(leaderboard_file_name), leaderboard(Leaderboard()) {

    read_player(player_file_name, player_name);  // Initialize player using the player.dat file
    read_space_grid(space_grid_file_name);  // Initialize the grid after the player is loaded
    read_celestial_objects(celestial_objects_file_name);  // Load celestial objects
    leaderboard.read_from_file(leaderboard_file_name);
}

// Function to read the space grid from a file
void AsteroidDash::read_space_grid(const string &input_file) {
    // TODO: Your code here
    
    std::ifstream space_grid_file(input_file);
    if (!space_grid_file.is_open()) {
        throw std::runtime_error("This file is not opened!"); 
    }

    std::string each_row;

    int row_number = 0;
    int col_number = 0;
    while (getline(space_grid_file, each_row)) {
        row_number++;
        col_number += each_row.size();
    }

    col_number = ((col_number / row_number) / 2) + 1;
    space_grid.resize(row_number, std::vector<int>(col_number));

    space_grid_file.close();
    space_grid_file.open(input_file);

    int row_index = 0;
    while (getline(space_grid_file, each_row))
    {
        int col_index = 0;
        for (char number : each_row) {
            if (number != ' ') {
                if (number == '1') {
                    space_grid[row_index][col_index] = 1;
                } else if (number == '0') {
                    space_grid[row_index][col_index] = 0;
                }
                col_index++;
            }
        }
        row_index++;
    }
    
}

// Function to read the player from a file
void AsteroidDash::read_player(const string &player_file_name, const string &player_name) {
    // TODO: Your code here

    std::ifstream player_file(player_file_name);
    if (!player_file.is_open()) {
        throw std::runtime_error("This file is not opened!"); 
    }

    vector<vector<bool>> player_shape;
    std::string each_row;

    int row_number = 0;
    int col_number = 0;
    bool at_the_beginning = true;
    while (getline(player_file, each_row))
    {   
        if (at_the_beginning) {
            int spacecharacter_place = each_row.find(' ');
            std::string row_string = each_row.substr(0, spacecharacter_place);
            std::string col_string = each_row.substr(spacecharacter_place + 1);
            row_number = std::stoi(row_string);
            col_number = std::stoi(col_string);
            at_the_beginning = false;
            continue;
        }

        std::vector<bool> row;
        for (char c : each_row) {
            if (c == '1' || c == '0') {
                row.push_back(c == '1');
            }
        }
        player_shape.push_back(row);
    }

    player = new Player(player_shape, row_number, col_number, player_name);
}

// Function to read celestial objects from a file
void AsteroidDash::read_celestial_objects(const string &input_file) {
    // TODO: Your code here
    
    std::ifstream celestial_object_file(input_file);
    if (!celestial_object_file.is_open()) {
        throw std::runtime_error("This file is not opened!"); 
    }

    std::string each_row;
    std::vector<std::string> celestial_object_attributes;

    while (getline(celestial_object_file, each_row)) {
        if (each_row == "") {
            if (!celestial_object_attributes.empty()) {
                generate_celestial_object(celestial_object_attributes);
                celestial_object_attributes.clear();
            }
            continue;
        } 
        celestial_object_attributes.push_back(each_row);
    }
    if (!celestial_object_attributes.empty()) {
        generate_celestial_object(celestial_object_attributes);
        celestial_object_attributes.clear();
    }
}


// Print the entire space grid
void AsteroidDash::print_space_grid() const {
    // TODO: Your code here
    std::cout << "Tick: " << game_time << std::endl;
    std::cout << "Lives: " << player->lives << std::endl;
    std::cout << "Ammo: " << player->current_ammo << std::endl;
    std::cout << "Score: " << current_score << std::endl;
    int row_number = static_cast<int>(space_grid.size());
    int col_number = static_cast<int>(space_grid[0].size());
    for (int i = 0; i < row_number; i++) {
        for (int j = 0; j < col_number; j++) {
            if (space_grid[i][j] == true) {
                std::cout << occupiedCellChar;
            } else if (space_grid[i][j] == false) {
                std::cout << unoccupiedCellChar;
            }
        }
        std::cout << std::endl;
        if (i == (row_number - 1)) {
            std::cout << std::endl;
        }
    }
}


// Function to update the space grid with player, celestial objects, and any other changes
// It is called in every game tick before moving on to the next tick.
void AsteroidDash::update_space_grid() {
    // TODO: Your code here

    // !!! süperrrrr çalışıyorsun beeeee
    clear_the_grid();
    load_to_player_on_the_grid();
    load_the_projectile();
    update_projectile_locations();
    load_to_celestialobjects_on_the_grid();
    update_celestial_locations();
    current_score++;
}

// Corresponds to the SHOOT command.
// It should shoot if the player has enough ammo.
// It should decrease the player's ammo
void AsteroidDash::shoot() {
    // TODO: Your code here

    if (player->current_ammo > 0) {
        player->current_ammo--;
        Projectile projectile;
        projectile.col_number = player->position_col + static_cast<int>(player->spacecraft_shape[0].size());
        projectile.row_number = player->position_row + (static_cast<int>(player->spacecraft_shape.size()) / 2);
        projectile_vector.push_back(projectile);
    }
}

// Destructor. Remove dynamically allocated member variables here.
AsteroidDash::~AsteroidDash() {
    // TODO: Your code here
}

// Function that generates a Celestial object according to the given properties and adds a list.
void AsteroidDash::generate_celestial_object(const std::vector<std::string> &celestial_object_attributes) {
    int row_counter = 0;
    int col_counter = 0;
    for (std::string row : celestial_object_attributes) {
        if (row[0] == 's') {
            break;
        } 
        col_counter += row.size();
        row_counter++;
    }
    if (col_counter != 0 && row_counter != 0) { 
        col_counter = (col_counter - 2) / row_counter;
    }

    std::vector<std::vector<bool>> celestial_shape(row_counter, std::vector<bool>(col_counter)); 

    int time_step = 0;
    int starting_row = 0;
    row_counter = 0;
    ObjectType element_type;
    for (std::string row : celestial_object_attributes) {
        if (row[0] == '[' || row[0] == '{') {
            element_type = ASTEROID;
            col_counter = 0;
            for (char c : row) {
                if (c == '[' || c == '{') {
                    continue;
                }
                int number = c - '0';
                if (number == 1 ) {
                    celestial_shape[row_counter][col_counter++] = true;
                } else {
                    celestial_shape[row_counter][col_counter++] = false;
                }
            }
            row_counter++;
        } else if (row[0] == 's') {
            starting_row = std::stoi(row.substr(2));
        } else if (row[0] == 't') {
            time_step = std::stoi(row.substr(2));
        } else if (row[0] == 'e') {
            if (row[2] == 'l') {
                element_type = LIFE_UP;
            } else if (row[2] == 'a') {
                element_type = AMMO;
            }
        } else {
            col_counter = 0;
            for (char c : row) {
                if (c == ']' || c == '}') {
                    continue;
                }
                int number = c - '0';
                if (number == 1 ) {
                    celestial_shape[row_counter][col_counter++] = true;
                } else {
                    celestial_shape[row_counter][col_counter++] = false;
                }
            }
            row_counter++;
        }
    }

    CelestialObject* new_celestial_object = new CelestialObject(celestial_shape, element_type, starting_row, time_step);
    
    new_celestial_object->starting_col = static_cast<int>(space_grid[0].size()) - 1;
    celestial_vector.push_back(new_celestial_object);


    if (celestial_objects_list_head == nullptr) {
        celestial_objects_list_head = new_celestial_object;
        current = celestial_objects_list_head;
    } else {
        current->next_celestial_object = new_celestial_object;
        if (current->right_rotation != nullptr) {
            current->right_rotation->next_celestial_object = new_celestial_object;
        }
        if (current->right_rotation->right_rotation != nullptr) {
            current->right_rotation->right_rotation->next_celestial_object = new_celestial_object;
        }
        if (current->right_rotation->right_rotation->right_rotation != nullptr) {
            current->right_rotation->right_rotation->right_rotation->next_celestial_object = new_celestial_object;
        }
        current = new_celestial_object;
    }
    
    CelestialObject* right_celestial_object = new CelestialObject(new_celestial_object);
    if (check_the_shapes(new_celestial_object->shape, right_celestial_object->shape)) {
        delete right_celestial_object; // !!! silmek için celestial içindeki siliciyi kullan 
        new_celestial_object->right_rotation = new_celestial_object;
        new_celestial_object->left_rotation = new_celestial_object;
    } else {
        new_celestial_object->right_rotation = right_celestial_object;
        right_celestial_object->left_rotation = new_celestial_object;
        right_celestial_object->right_rotation = new_celestial_object;
        new_celestial_object->left_rotation = right_celestial_object;
        CelestialObject* right_right_celestial_object = new CelestialObject(right_celestial_object);
        if (check_the_shapes(new_celestial_object->shape, right_right_celestial_object->shape)) {
            delete right_right_celestial_object; // !!! silmek için celestial içindeki siliciyi kullan 
        } else {
            new_celestial_object->left_rotation = right_right_celestial_object;
            right_celestial_object->right_rotation =  right_right_celestial_object;
            right_right_celestial_object->left_rotation = right_celestial_object;
            right_right_celestial_object->right_rotation = new_celestial_object;

            CelestialObject* right_right_right_celestial_object = new CelestialObject(right_right_celestial_object);
            right_right_celestial_object->right_rotation = right_right_right_celestial_object;
            right_right_right_celestial_object->left_rotation = right_right_celestial_object;
            new_celestial_object->left_rotation = right_right_right_celestial_object;
            right_right_right_celestial_object->right_rotation = new_celestial_object;
        }
    }
    
}

// Function to check the similarity between the right rotation object and the original object.
bool AsteroidDash::check_the_shapes(const std::vector<std::vector<bool>> &original_shape, const std::vector<std::vector<bool>> &copied_shape) {
    if (original_shape.size() != copied_shape.size()) {
        return false;
    }
    for (int i = 0; i < static_cast<int>(original_shape.size()); i++) {
        for (int j = 0; j < static_cast<int>(original_shape[0].size()); j++) {
            if (original_shape[i][j] != copied_shape[i][j]) {
                return false;
            }
        }
    }
    return true;
}

// Function to loads the player on the grid.
void AsteroidDash::load_to_player_on_the_grid() {
    for (int i = 0; i < static_cast<int>(player->spacecraft_shape.size()); i++) {
        for (int j = 0; j < static_cast<int>(player->spacecraft_shape[0].size()); j++) {
            space_grid[player->position_row + i][player->position_col + j] = player->spacecraft_shape[i][j];
        }
    }
}

// Function to loads the celestial objects on the grid.
void AsteroidDash::load_to_celestialobjects_on_the_grid() {
    if (celestial_vector.empty()) {
        return;
    }
    for (CelestialObject* current_celestial_object : celestial_vector) {
        if (current_celestial_object->time_of_appearance <= static_cast<int>(game_time)) {
            if (current_celestial_object->was_it_hit) {
                break;
            }
            if (current_celestial_object->starting_row < 0) {
                break;
            }
            if (static_cast<int>(space_grid.size()) < static_cast<int>(current_celestial_object->shape.size()) + current_celestial_object->starting_row) {
                break;
            }  
            int time_difference = game_time - current_celestial_object->time_of_appearance;
            for(int i = 0; i < static_cast<int>(current_celestial_object->shape.size()); i++) {
                for(int j = 0; j <= time_difference; j++) {
                    if (static_cast<int>(current_celestial_object->shape[0].size()) <= j) {
                        break;
                    }
                    if (current_celestial_object->starting_col + j < 0) {
                        continue;
                    }
                    if (space_grid[current_celestial_object->starting_row + i][current_celestial_object->starting_col + j] == true) {
                        if (is_there_a_collision_with_a_projectile(current_celestial_object->starting_row + i, current_celestial_object->starting_col + j)) {
                            //collision with projectile
                            current_score += 10;
                        } else {
                            if (current_celestial_object->object_type == ASTEROID && current_celestial_object->shape[i][j] == true) {
                                if (player->lives == 1) {
                                    player->lives--;
                                    current_celestial_object->was_it_hit = true;
                                    game_over = true;
                                }
                                current_celestial_object->was_it_hit = true;
                            } else if (current_celestial_object->object_type == LIFE_UP && current_celestial_object->shape[i][j] == true) {
                                current_celestial_object->was_it_hit = true;
                                player->lives++;
                                break;
                            } else if (current_celestial_object->object_type == AMMO && current_celestial_object->shape[i][j] == true) {
                                current_celestial_object->was_it_hit = true;
                                player->current_ammo = player->max_ammo;
                                break;
                            }
                        }
                        
                    } else {
                        space_grid[current_celestial_object->starting_row + i][current_celestial_object->starting_col + j] = current_celestial_object->shape[i][j];
                    }
                }
            }
            if (current_celestial_object->was_it_hit) {
                clear_the_celestial_objects_locations(current_celestial_object);
                continue;
            }
        }
    }
}

// Function to cleans the grid.
void AsteroidDash::clear_the_grid() {
    for (int i = 0; i < static_cast<int>(space_grid.size()); i++) {
        for (int j = 0; j < static_cast<int>(space_grid[0].size()); j++) {
            space_grid[i][j] = false;
        }
    }
}


// Function to update the celestial objects' locations.
void AsteroidDash::update_celestial_locations() {
    for (CelestialObject* current_celestial_object : celestial_vector) {
        if (current_celestial_object->time_of_appearance <= static_cast<int>(game_time)) {
            current_celestial_object->starting_col--;
        }
        
    }
}

// Function to load the projectile.
void AsteroidDash::load_the_projectile() {
    if (!projectile_vector.empty()) {
        for (Projectile projectile : projectile_vector) {
            space_grid[projectile.row_number][projectile.col_number] = projectile.type;
        }
    }
}

// Function to update the locations of projectiles
void AsteroidDash::update_projectile_locations() {
    if (!projectile_vector.empty()) {
        for (Projectile& projectile : projectile_vector) {
            if (projectile.col_number < static_cast<int>(space_grid[0].size()) - 1) {
                projectile.col_number++;
            } else if (projectile.col_number == static_cast<int>(space_grid[0].size()) - 1) {
                projectile_vector.erase(projectile_vector.begin());
            }
        }
    }
}

// Function to controlling the collision of a spaceship with a celestial object.
bool AsteroidDash::is_there_a_collision_with_a_projectile(int row, int col) {
    for (Projectile current_projectile : projectile_vector) {
        if (current_projectile.row_number == row && current_projectile.col_number == col) {
            return true;
        }
    }
    return false;
}

void AsteroidDash::clear_the_celestial_objects_locations(CelestialObject* celestial_object) {

    int time_difference = game_time - celestial_object->time_of_appearance;
    for(int i = 0; i < static_cast<int>(celestial_object->shape.size()); i++) {
        for(int j = 0; j <= time_difference; j++) {
            if (static_cast<int>(celestial_object->shape[0].size()) <= j) {
                break;
            }
            if (celestial_object->starting_col + j < 0) {
                break;
            }
            space_grid[celestial_object->starting_row + i][celestial_object->starting_col + j] = false;
        }
    }
    load_to_player_on_the_grid();
}