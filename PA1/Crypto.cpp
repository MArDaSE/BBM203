#include "Crypto.h"
#include "GrayscaleImage.h"


// Extract the least significant bits (LSBs) from SecretImage, calculating x, y based on message length
std::vector<int> Crypto::extract_LSBits(SecretImage& secret_image, int message_length) {
    std::vector<int> LSB_array;
    // TODO: Your code goes here.

    // 1. Reconstruct the SecretImage to a GrayscaleImage.
    GrayscaleImage image = secret_image.reconstruct();
    // 2. Calculate the image dimensions.
    int width = image.get_width();
    int heigth = image.get_height();
    // 3. Determine the total bits required based on message length.
    int bits_amount = message_length * 7;
    // 4. Ensure the image has enough pixels; if not, throw an error.
    int pixels_amount = width * heigth;
    if (pixels_amount < bits_amount) {
        throw std::runtime_error("You do not have enough pixels!");
    }
    // 5. Calculate the starting pixel from the message_length knowing that  
    //    the last LSB to extract is in the last pixel of the image.
    int starting_pixel_index = pixels_amount - bits_amount;
    // 6. Extract LSBs from the image pixels and return the result.
    for (int i = starting_pixel_index; i < pixels_amount; i++) {
        int lsb = image.get_pixel(i / width, i % width) & 1;
        LSB_array.push_back(lsb);
    }
    return LSB_array;
}


// Decrypt message by converting LSB array into ASCII characters
std::string Crypto::decrypt_message(const std::vector<int>& LSB_array) {
    std::string message;
    // TODO: Your code goes here.

    // 1. Verify that the LSB array size is a multiple of 7, else throw an error.
    if ((LSB_array.size() % 7) != 0) {
        throw std::runtime_error("Your size of vector is not multiple of 7!");
    }
    // 2. Convert each group of 7 bits into an ASCII character.
    // 3. Collect the characters to form the decrypted message.
    std::string asci_charecters = "";
    int counter = 0;
    for (size_t i = 0; i < LSB_array.size(); i++) {
        counter++;
        asci_charecters += std::to_string(LSB_array[i]);
        if (counter == 7) {
             std::bitset<7> binary_number(asci_charecters);
             unsigned long integer_number = binary_number.to_ulong();
             char asci_charecter = static_cast<char>(integer_number);
             message += asci_charecter;
             asci_charecters = "";
             counter = 0;
        }
    }
    // 4. Return the resulting message.

    return message;
}

// Encrypt message by converting ASCII characters into LSBs
std::vector<int> Crypto::encrypt_message(const std::string& message) {
    std::vector<int> LSB_array;
    // TODO: Your code goes here.

    // 1. Convert each character of the message into a 7-bit binary representation.
    //    You can use std::bitset.
    // 2. Collect the bits into the LSB array.
    for (size_t i = 0; i < message.size(); i++) {
        std::bitset<7> bit_representation(message[i]);
         for (int each_bit = 6; 0 <= each_bit; each_bit--) {
            LSB_array.push_back(bit_representation[each_bit]);
         }  
    }
    // 3. Return the array of bits.

    return LSB_array;
}

// Embed LSB array into GrayscaleImage starting from the last bit of the image
SecretImage Crypto::embed_LSBits(GrayscaleImage& image, const std::vector<int>& LSB_array) {
    // TODO: Your code goes here.

    // 1. Ensure the image has enough pixels to store the LSB array, else throw an error.
    int pixels_numbers = image.get_width() * image.get_width();
    int message_bit_numbers =  LSB_array.size();
    if (pixels_numbers < message_bit_numbers) {
        throw std::runtime_error("The image is not large enough to embed the message!");
    }
    // 2. Find the starting pixel based on the message length knowing that  
    //    the last LSB to embed should end up in the last pixel of the image.
    int starting_pixel_index = pixels_numbers - message_bit_numbers;
    // 3. Iterate over the image pixels, embedding LSBs from the array.
    int counter = 0;
    for (int i = starting_pixel_index; i < pixels_numbers; i++) {
        int image_pixel = image.get_pixel(i / image.get_width(), i % image.get_width());
        int lsb = image_pixel & 1;
        if (lsb != LSB_array[counter]) {
            std::bitset<8> binary_number(image_pixel);
            std::string updated_number = binary_number.to_string();
            updated_number[7] = LSB_array[counter] + '0';
            std::bitset<8> binary_bits(updated_number);
            unsigned long integer_value = binary_bits.to_ulong();
            image.set_pixel(i / image.get_width(), i % image.get_width(), integer_value);
        }
        counter++;
    }
    // 4. Return a SecretImage object constructed from the given GrayscaleImage 
    //    with the embedded message.
    SecretImage secret_image(image);
    return secret_image;
}