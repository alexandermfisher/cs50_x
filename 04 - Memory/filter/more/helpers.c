#include <math.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

#define MAX_COLOUR_VALUE 255                // max colour itensity any component (e.g. red) can be assigned
#define BOX_LENGTH 3                        // box length for kernels and blurring are 3x3 which enclose current pixel centrally i.e 0,0

// define Gx and Gy kernels
const int Gx[3][3] =    {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
const int Gy[3][3] =    {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // loop over each pixel in image
    for (int i = 0; i < height; ++i) {
        int average = 0;
        for (int j= 0; j < width; ++j) {
            // calculate average colour intensity and assign to pixel
            average = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // loop over pixels in image
    for (int i = 0; i < height; ++i) {
        for (int j= 0; j < width; ++j) {
            // calculate new pixel value and assign to current pixel if less than 255 or if not assigning 255 instead.
            RGBTRIPLE original_pixel = image[i][j];
            int blue_value = round(0.272 * original_pixel.rgbtRed + 0.534 * original_pixel.rgbtGreen + 0.131 * original_pixel.rgbtBlue);
            if (blue_value > MAX_COLOUR_VALUE) {
                image[i][j].rgbtBlue = MAX_COLOUR_VALUE;
            } else {
                image[i][j].rgbtBlue = blue_value;
            }

            int green_value = round(0.349 * original_pixel.rgbtRed + 0.686 * original_pixel.rgbtGreen + 0.168 * original_pixel.rgbtBlue);
            if (green_value > MAX_COLOUR_VALUE) {
                image[i][j].rgbtGreen = MAX_COLOUR_VALUE;
            } else {
                image[i][j].rgbtGreen = green_value;
            }

            int red_value = round(0.393 * original_pixel.rgbtRed + 0.769 * original_pixel.rgbtGreen + 0.189 * original_pixel.rgbtBlue);
            if (red_value > MAX_COLOUR_VALUE) {
                image[i][j].rgbtRed = MAX_COLOUR_VALUE;
            } else {
                image[i][j].rgbtRed = red_value;
            }
        }
    }

    return;
}


void reverse_row(int width, int ith_pixel, RGBTRIPLE row_of_pixels[width]);

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // loop over each row of the image and reverse row.
    for (int row = 0; row < height; ++row) {
        reverse_row(width, 0, image[row]);
    }

    return;
}

void reverse_row(int width, int ith_pixel, RGBTRIPLE row_of_pixels[width])
{
    // base case: end of an array is reached or array index out-of-bounds
    if (ith_pixel >= width) {
        return;
    }
    // store a copy of ith pixel
    RGBTRIPLE pixel = row_of_pixels[ith_pixel];

    // reach the end of the array using recursion i.e. all pixels are now stored and orignal array can now be written over.
    reverse_row(width, ++ith_pixel, row_of_pixels);

    // put pixels back into row in reverse order
    row_of_pixels[width - ith_pixel] = pixel;

    return;
}


void update_pixel_blur(int pixel_i, int pixel_j, int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE copy[height][width]);

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // allocate space to make a copy of the original image.
    RGBTRIPLE (*original_image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (!original_image) {
        fprintf(stderr, "Failed to allocate enough memory for a copy of original image\n");
        return;
    }

    // copy image to original_image for calculations
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            original_image[i][j] = image[i][j];
        }
    }
    
    // loop over each pixel in image and calculate new pixel value and update image[i][j]
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
                update_pixel_blur(i, j, height, width, image, original_image);
        }
    }

    free(original_image);
    
    return;
}

// calculates the average colour denisty for a given pixels using its neigbours and then unpdates image accordingly
void update_pixel_blur(int pixel_i, int pixel_j, int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE copy[height][width])
{
    // average colour values initialised at 0. index map: (0,1,2) --> (BlUE, GREEN, RED)
    int average_colour_values[3] = {0, 0, 0};
    int count_pixels_included = 0;

    // loop over a 3*3 box with current pixel in the centre
    for (int i = - 1; i < BOX_LENGTH - 1; ++i) {
        for (int j = - 1; j < 2; ++j) {
            // check to see if neigbouring pixel i of centred pixel located at (pixel__i, pixel_j) in valid domain of image, 
            // if so increment both colour value and pixels included counts
            if (((pixel_i + i) >= 0) && ((pixel_i + i) < height) && ((pixel_j + j) >= 0) && ((pixel_j + j) < width)) {
                average_colour_values[0] += copy[pixel_i + i][pixel_j + j].rgbtBlue;
                average_colour_values[1] += copy[pixel_i + i][pixel_j + j].rgbtGreen;
                average_colour_values[2] += copy[pixel_i + i][pixel_j + j].rgbtRed;
                count_pixels_included += 1;
            }
        }
    }

    // complete the calculation for the new pixel by taking average and also cap colour values to MAX_COLOUR_VALUE i.e. 255.
    RGBTRIPLE updated_pixel;

    int blue_value = round(average_colour_values[0] / (float) count_pixels_included);
    if (blue_value > MAX_COLOUR_VALUE) {
        updated_pixel.rgbtBlue = MAX_COLOUR_VALUE;
    } else {
        updated_pixel.rgbtBlue = blue_value;
    }

    int green_value = round(average_colour_values[1] / (float) count_pixels_included);
    if (green_value > MAX_COLOUR_VALUE) {
        updated_pixel.rgbtGreen = MAX_COLOUR_VALUE;
    } else {
        updated_pixel.rgbtGreen = green_value;
    }

    int red_value = round(average_colour_values[2] / (float) count_pixels_included);
    if (red_value > MAX_COLOUR_VALUE) {
        updated_pixel.rgbtRed = MAX_COLOUR_VALUE;
    } else {
        updated_pixel.rgbtRed = red_value;
    }

    // update images pixel
    image[pixel_i][pixel_j] = updated_pixel;

    return;
}


void update_pixel_edge(
            int height_index, int width_index, int height,
            int width, RGBTRIPLE image[height][width], RGBTRIPLE copy[height][width]);

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // allocate space to make a copy of the original image.
    RGBTRIPLE (*original_image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (!original_image) {
        fprintf(stderr, "Failed to allocate enough memory for a copy of original image\n");
        return;
    }

    // copy image to original_image for calculations
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            original_image[i][j] = image[i][j];
        }
    }

    // loop over each pixel in image and calculate new pixel using Sobel operator.
    for (int height_index = 0; height_index < height; ++height_index) {
        for (int width_index = 0; width_index < width; ++width_index) {
            update_pixel_edge(height_index, width_index, height, width, image, original_image);
        }
    }

    free(original_image);
    return;
}


void apply_kernels(
        int Gx_sum[BOX_LENGTH], int Gy_sum[BOX_LENGTH],
        int pixel_i, int pixel_j, int kernel_i, int kernel_j, int width, RGBTRIPLE (*image)[width]);

// apply kernels to pixel and neighbouring pixels and update pixel in image.
void update_pixel_edge(
            int height_index, int width_index, int height,
            int width, RGBTRIPLE image[height][width], RGBTRIPLE copy[height][width])
{
    // initialise a array to store summations for colour values ordered Blue, Green, Red.
    int Gx_sum[BOX_LENGTH] = {0, 0, 0};
    int Gy_sum[BOX_LENGTH] = {0, 0, 0};

    // loop over the 3*3 box with current pixel (under modification) in the centre (i = 0, j = 0)
    for (int i = -1; i < BOX_LENGTH - 1; ++i) {
        for (int j = -1; j < BOX_LENGTH - 1; ++j) {
            // check to see if neigbouring pixel is in valid domain of image, and apply kernel for that single pixel.
            if (((height_index + i) >= 0) && ((height_index + i) < height) && ((width_index + j) >= 0) && ((width_index + j) < width)) {
                apply_kernels(Gx_sum, Gy_sum, height_index + i, width_index + j, i, j, width, copy);
            }
        }
    }

    // calculate red, green and blue values, capping values at 255.
    float blue_value  = round(sqrt((float)(Gx_sum[0] * Gx_sum[0]) + (Gy_sum[0] * Gy_sum[0])));
    float green_value = round(sqrt((float)(Gx_sum[1] * Gx_sum[1]) + (Gy_sum[1] * Gy_sum[1])));
    float red_value   = round(sqrt((float)(Gx_sum[2] * Gx_sum[2]) + (Gy_sum[2] * Gy_sum[2])));

    if (blue_value > MAX_COLOUR_VALUE){
        blue_value = MAX_COLOUR_VALUE;
    }
    if (green_value > MAX_COLOUR_VALUE){
        green_value = MAX_COLOUR_VALUE;
    }
    if (red_value > MAX_COLOUR_VALUE){
        red_value = MAX_COLOUR_VALUE;
    }

    // finally assign these values to image pixel
    image[height_index][width_index].rgbtBlue = blue_value;
    image[height_index][width_index].rgbtGreen = green_value;
    image[height_index][width_index].rgbtRed = red_value;
}

void apply_kernels(
        int Gx_sum[BOX_LENGTH], int Gy_sum[BOX_LENGTH],
        int pixel_i, int pixel_j, int kernel_i, int kernel_j, int width, RGBTRIPLE (*image)[width])
{
    // shift kernel indexes back to the starting at 0 by adding 1
     ++kernel_i; ++kernel_j;

    // apply Gx kernel
    Gx_sum[0] += (Gx[kernel_i][kernel_j] * image[pixel_i][pixel_j].rgbtBlue);
    Gx_sum[1] += (Gx[kernel_i][kernel_j] * image[pixel_i][pixel_j].rgbtGreen);
    Gx_sum[2] += (Gx[kernel_i][kernel_j] * image[pixel_i][pixel_j].rgbtRed);

    // apply Gy kernel
    Gy_sum[0] += (Gy[kernel_i][kernel_j] * image[pixel_i][pixel_j].rgbtBlue);
    Gy_sum[1] += (Gy[kernel_i][kernel_j] * image[pixel_i][pixel_j].rgbtGreen);
    Gy_sum[2] += (Gy[kernel_i][kernel_j] * image[pixel_i][pixel_j].rgbtRed);

    return;
}