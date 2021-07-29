#include <math.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

#define MAX_COLOUR_VALUE 255                // max colour itensity any component (e.g. red) can be assigned.
#define BOX_LENGTH 3                        // box for blurring is a 3x3 which encloses pixel that is being adjusted to average.

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // loop over each pixel in image
    for (int i = 0; i < height; ++i)
    {
        int average = 0;
        for (int j= 0; j < width; ++j)
        {
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
    for (int i = 0; i < height; ++i)
    {
        for (int j= 0; j < width; ++j)
        {
            // calculate new pixel value and assign to current pixel if less than 255 or if not assigning 255 instead.
            RGBTRIPLE original_pixel = image[i][j];
            int blue_value = round(0.272 * original_pixel.rgbtRed + 0.534 * original_pixel.rgbtGreen + 0.131 * original_pixel.rgbtBlue);
            if (blue_value > MAX_COLOUR_VALUE){
                image[i][j].rgbtBlue = MAX_COLOUR_VALUE;
            } else{
                image[i][j].rgbtBlue = blue_value;
            }

            int green_value = round(0.349 * original_pixel.rgbtRed + 0.686 * original_pixel.rgbtGreen + 0.168 * original_pixel.rgbtBlue);
            if (green_value > MAX_COLOUR_VALUE){
                image[i][j].rgbtGreen = MAX_COLOUR_VALUE;
            } else{
                image[i][j].rgbtGreen = green_value;
            }

            int red_value = round(0.393 * original_pixel.rgbtRed + 0.769 * original_pixel.rgbtGreen + 0.189 * original_pixel.rgbtBlue);
            if (red_value > MAX_COLOUR_VALUE){
                image[i][j].rgbtRed = MAX_COLOUR_VALUE;
            } else{
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
    for (int row = 0; row < height; ++row)
    {
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

void update_pixel(int pixel_i, int pixel_j, int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE copy[height][width]);

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
                update_pixel(i, j, height, width, image, original_image);
        }
    }

    free(original_image);
    
    return;
}

// calculates the average colour denisty for a given pixels using its neigbours and then unpdates image accordingly
void update_pixel(int pixel_i, int pixel_j, int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE copy[height][width])
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