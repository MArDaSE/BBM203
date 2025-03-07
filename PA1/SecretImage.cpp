#include "SecretImage.h"


// Constructor: split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage& image) {
    // TODO: Your code goes here.
    // 1. Dynamically allocate the memory for the upper and lower triangular matrices.
    this->width = image.get_width();
    this->height = image.get_height();
    int area_value_for_upper = (width * (width + 1)) / 2;
    int area_value_for_lower = (width * (width - 1)) / 2;
    this->upper_triangular = new int[area_value_for_upper];
    this->lower_triangular = new int[area_value_for_lower];
    // 2. Fill both matrices with the pixels from the GrayscaleImage.
    int row = 0;
    int col = 0;
    int counter = width - 1;
    for (int i = 0; i < area_value_for_upper; i++) {
        upper_triangular[i] = image.get_pixel(row, col);
        col++;
        if (col == width) {
            row++;
            col = col - counter;
            counter--;
        }
    }
    row = 1;
    col = 0;
    for (int i = 0; i < area_value_for_lower; i++) {
        lower_triangular[i] = image.get_pixel(row, col);
        col++;
        if (col == row) {
            col = 0;
            row++;
        }
    }
}

// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int * upper, int * lower) {
    // TODO: Your code goes here.
    // Since file reading part should dynamically allocate upper and lower matrices.
    // You should simply copy the parameters to instance variables.
    this->width = w;
    this->height = h;
    this->upper_triangular = upper;
    this->lower_triangular = lower;
}


// Destructor: free the arrays
SecretImage::~SecretImage() {
    // TODO: Your code goes here.
    // Simply free the dynamically allocated memory
    // for the upper and lower triangular matrices.
    delete[] upper_triangular;
    delete[] lower_triangular;
}

// Reconstructs and returns the full image from upper and lower triangular matrices.
GrayscaleImage SecretImage::reconstruct() const {
    GrayscaleImage image(width, height);

    // TODO: Your code goes here.

    int area_value_for_upper = (width * (width + 1)) / 2;
    int area_value_for_lower = (width * (width - 1)) / 2;
    int row = 0;
    int col = 0;
    int counter = width - 1;
    for (int i = 0; i < area_value_for_upper; i++) {
        image.set_pixel(row, col, upper_triangular[i]);
        col++;
        if (col == width) {
            row++;
            col = col - counter;
            counter--;
        }
    }
    row = 1;
    col = 0;
    for (int i = 0; i < area_value_for_lower; i++) {
        image.set_pixel(row, col, lower_triangular[i]);
        col++;
        if (col == row) {
            col = 0;
            row++;
        }
    }

    return image;
}

// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage& image) {
    // TODO: Your code goes here.
    // Update the lower and upper triangular matrices 
    // based on the GrayscaleImage given as the parameter.
    int area_value_for_upper = (width * (width + 1)) / 2;
    int area_value_for_lower = (width * (width - 1)) / 2;
    int row = 0;
    int col = 0;
    int counter = width - 1;
    for (int i = 0; i < area_value_for_upper; i++) {
        upper_triangular[i] = image.get_pixel(row, col);
        col++;
        if (col == width) {
            row++;
            col = col - counter;
            counter--;
        }
    }
    row = 1;
    col = 0;
    for (int i = 0; i < area_value_for_lower; i++) {
        lower_triangular[i] = image.get_pixel(row, col);
        col++;
        if (col == row) {
            col = 0;
            row++;
        }
    }
}

// Save the upper and lower triangular arrays to a file
void SecretImage::save_to_file(const std::string& filename) {
    // TODO: Your code goes here.
    // 1. Write width and height on the first line, separated by a single space.
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "File could not open!" << std::endl;
        return;
    }
    file << this->width << " " << this->height << std::endl;
    // 2. Write the upper_triangular array to the second line.
    // Ensure that the elements are space-separated. 
    // If there are 15 elements, write them as: "element1 element2 ... element15"
    int area_value_for_upper = (this->width * (this->width + 1)) / 2;
    int area_value_for_lower = (this->width * (this->width - 1)) / 2;

    for (int i = 0; i < area_value_for_upper; i++) {
        file << this->upper_triangular[i];
        if (i != (area_value_for_upper - 1)) {
            file << " ";
        }
    }
    file << std::endl;
    // 3. Write the lower_triangular array to the third line in a similar manner
    // as the second line.
    for (int i = 0; i < area_value_for_lower; i++) {
        file << this->lower_triangular[i];
        if (i != (area_value_for_lower - 1)) {
            file << " ";
        }
    }
    file.close();
}

// Static function to load a SecretImage from a file
SecretImage SecretImage::load_from_file(const std::string& filename) {
    // TODO: Your code goes here.
    // 1. Open the file and read width and height from the first line, separated by a space.
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "File could not open!" << std::endl;
    }
    int width, height;
    file >> width >> height;
    // 2. Calculate the sizes of the upper and lower triangular arrays.
    int area_value_for_upper, area_value_for_lower;
    for (int i = width; 0 < i; i--) {
        area_value_for_upper += i;
    }
    area_value_for_lower = (width * height) - area_value_for_upper;
    // 3. Allocate memory for both arrays.
    int *upper_triangular = new int[area_value_for_upper];
    int *lower_triangular = new int[area_value_for_lower]; 
    // 4. Read the upper_triangular array from the second line, space-separated.
    for (int i = 0; i < area_value_for_upper; i++) {
        file >> upper_triangular[i];
    }
    // 5. Read the lower_triangular array from the third line, space-separated.
    for (int i = 0; i < area_value_for_lower; i++) {
        file >> lower_triangular[i];
    }
    // 6. Close the file and return a SecretImage object initialized with the
    //    width, height, and triangular arrays.
    file.close();
    SecretImage secret_image(width, height, upper_triangular, lower_triangular);
    return secret_image;
}

// Returns a pointer to the upper triangular part of the secret image.
int * SecretImage::get_upper_triangular() const {
    return upper_triangular;
}

// Returns a pointer to the lower triangular part of the secret image.
int * SecretImage::get_lower_triangular() const {
    return lower_triangular;
}

// Returns the width of the secret image.
int SecretImage::get_width() const {
    return width;
}

// Returns the height of the secret image.
int SecretImage::get_height() const {
    return height;
}
