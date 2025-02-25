#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>
#include <math.h>

// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage& image, int kernelSize) {
    // TODO: Your code goes here.
    // 1. Copy the original image for reference.
    GrayscaleImage copy_image = image;
    // 2. For each pixel, calculate the mean value of its neighbors using a kernel.
    for (int i = 0; i < image.get_height(); i++) {
        for (int j = 0; j < image.get_width(); j++) {
            int mean_value = 0;
            int min_row = i - (kernelSize / 2);
            int max_row = i + (kernelSize / 2);
            int min_col = j - (kernelSize / 2);
            int max_col = j + (kernelSize / 2);
            for (int k = min_row; k <= max_row; k++) {
                for (int m = min_col; m <= max_col; m++) {
                    if ((0 <= k && k < image.get_height()) && (0 <= m && m < image.get_width())) {
                        mean_value += image.get_pixel(k, m);
                    }
                }
            }
            copy_image.set_pixel(i, j, (mean_value / (kernelSize * kernelSize)));          
        }
    }
    // 3. Update each pixel with the computed mean.
    for (int i = 0; i < image.get_height(); i++) {
        for (int j = 0; j < image.get_width(); j++) {
            image.set_pixel(i, j, copy_image.get_pixel(i, j));
        }
    }
    return;
}

// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize, double sigma) {
    // TODO: Your code goes here.
    // 1. Create a Gaussian kernel based on the given sigma value.
    double** gaussian_kernel = new double*[kernelSize];
    for (int row = 0; row < kernelSize; row++) {
        gaussian_kernel[row] = new double[kernelSize];
    } 
    int kernel_center = kernelSize / 2;
    double kernel_sum = 0;
    for (int i = 0; i < kernelSize; i++) {
        for (int j = 0; j < kernelSize; j++) {
            int x_destination = i - kernel_center;
            int y_destination = j - kernel_center;
            double üstel = 2.0 * M_PI * sigma * sigma;
            double radius = x_destination * x_destination + y_destination * y_destination;
            double alt = 2.0 * sigma * sigma;
            gaussian_kernel[i][j] = (1.0 / üstel) * std::exp(-radius / alt);
            kernel_sum += gaussian_kernel[i][j];
        }
    }
    // 2. Normalize the kernel to ensure it sums to 1.
    for (int i = 0; i < kernelSize; i++) {
        for (int j = 0; j < kernelSize; j++) {
            gaussian_kernel[i][j] = gaussian_kernel[i][j] / kernel_sum;
        }
    }
    // 3. For each pixel, compute the weighted sum using the kernel.
    GrayscaleImage copy_image = image;
    for (int i = 0; i < image.get_height(); i++) {
        for (int j = 0; j < image.get_width(); j++) {
            double gaussian_value = 0.0;
            int counter = 0;
            int min_row = i - (kernelSize / 2);
            int max_row = i + (kernelSize / 2);
            int min_col = j - (kernelSize / 2);
            int max_col = j + (kernelSize / 2);
            for (int k = min_row; k <= max_row; k++) {
                for (int m = min_col; m <= max_col; m++) {
                    if ((0 <= k && k < image.get_height()) && (0 <= m && m < image.get_width())) {
                        gaussian_value += image.get_pixel(k, m) * gaussian_kernel[counter / kernelSize][counter % kernelSize];
                    }
                    counter++;
                }
            }
            copy_image.set_pixel(i, j, (int)gaussian_value);
        }
    }
    // 4. Update the pixel values with the smoothed results.
    for (int i = 0; i < image.get_height(); i++) {
        for (int j = 0; j < image.get_width(); j++) {
            image.set_pixel(i, j, copy_image.get_pixel(i, j));
        }
    }
    
    for (int i = 0; i < kernelSize; ++i) {
        delete[] gaussian_kernel[i];
    }
    delete[] gaussian_kernel; 
    return;
}

// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage& image, int kernelSize, double amount) {
    // TODO: Your code goes here.
    // 1. Blur the image using Gaussian smoothing, use the default sigma given in the header.
    GrayscaleImage gaussian_image = image;
    apply_gaussian_smoothing(gaussian_image, kernelSize, 1.0);
    // 2. For each pixel, apply the unsharp mask formula: original + amount * (original - blurred).
    // 3. Clip values to ensure they are within a valid range [0-255].
    for (int i = 0; i < image.get_height(); i++) {
        for (int j = 0; j < image.get_width(); j++) {
            double original_pixel = image.get_pixel(i, j);
            double gaussian_pixel = gaussian_image.get_pixel(i, j);
            int result_image = (int)(original_pixel + amount * (original_pixel - gaussian_pixel));
            if (result_image < 0) {
                image.set_pixel(i, j, 0);
            } else if (255 < result_image) {
                image.set_pixel(i, j, 255);
            } else {
                image.set_pixel(i, j, result_image);
            }
        }
    }
    return;
}
