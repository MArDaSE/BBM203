#include "CelestialObject.h"


// Constructor to initialize CelestialObject with essential properties
CelestialObject::CelestialObject(const vector<vector<bool>> &shape, ObjectType type, int start_row,
                                 int time_of_appearance)
        : shape(shape), object_type(type), starting_row(start_row), time_of_appearance(time_of_appearance) {
    // TODO: Your code here  
}


// Copy constructor for CelestialObject
CelestialObject::CelestialObject(const CelestialObject *other)
        : shape(other->shape),  // Copy the 2D vector shape
          object_type(other->object_type),  // Copy the object type
          starting_row(other->starting_row),  // Copy the starting row
          time_of_appearance(other->time_of_appearance)  // Copy the time of appearance
{
    // TODO: Your code here
    
    this->shape = generate_right_rotation(this->shape);
    this->starting_col = other->starting_col;
}

// Function to delete rotations of a given celestial object. It should free the dynamically allocated
// memory for each rotation.
void CelestialObject::delete_rotations(CelestialObject *target) {
    // TODO: Your code here
}

// Function to adds the right rotation of the object
std::vector<std::vector<bool>> CelestialObject::generate_right_rotation(const vector<vector<bool>> &object_shape) {
    int row_size = object_shape.size();
    int col_size = object_shape[0].size();
    
    std::vector<bool> temp(row_size * col_size); // !!! sırayla tutmak için

    int counter = 0;
    for (int i = (row_size - 1); 0 <= i; i--) {
        for (int j = 0; j < col_size; j++) { // !!! en alt satırdan alarak yukari doğru çıkıyor.
            temp[counter++] = object_shape[i][j];
        }
    }

    // !!! tersledim burada, row size ile col size çaprazlandi yani [3][2] --> [2][3] oldu.
    row_size = object_shape[0].size();
    col_size = object_shape.size();

    std::vector<std::vector<bool>> right_rotation_shape(row_size, std::vector<bool>(col_size));
    int boolean_counter = 0;
    for (int j = 0; j < col_size; j++) {
        for (int i = 0; i < row_size; i++) {
            right_rotation_shape[i][j] = temp[boolean_counter++];
        }
    }

    return right_rotation_shape;
}