#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"

int tops(int component)
{
    if (component >= 255)
    {
        return 255;
    }
    else
    {
        return component;
    }
}

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int avg;
    // Goes throug all the pixels of the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {   
            // Computes the average of the RGB components 
            avg = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            // Asigns the average of the RGB components 
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Allocates memory for a temporary reflected image
    RGBTRIPLE(*reflected)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (reflected == NULL)
    {
        fprintf(stderr, "Not enough memory to store reflected image.\n");
        return;
    }
    // Fill the temporary image with the modified pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            reflected[i][(width - 1) - j] = image[i][j];
        }
    }
    // Assigns the new values to the image pointer
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = reflected[i][j];
        }
    }
    // Frees the alocated memory for the temporary image
    free(reflected);
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Creates variables for the filter's computed values
    int R, G, B;
    // Allocates memory for a temporary blurred image
    RGBTRIPLE(*blurred)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (blurred == NULL)
    {
        fprintf(stderr, "Not enough memory to store blured image.\n");
        return;
    }
    // Loops through all pixels in the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Validate if the current pixel is not on the upper or lower edges
            if (0 < i && i < height - 1) // Centrales
            {
                // Validate if the current pixel is not on the left or right edges
                if (0 < j && j < width - 1)  // Centrales
                {
                    B = 0; 
                    G = 0; 
                    R = 0;
                    // Evaluate the blue component average over 3x3 grid
                    B = round((image[i - 1][j - 1].rgbtBlue +
                               image[i - 1][j].rgbtBlue +
                               image[i - 1][j + 1].rgbtBlue +
                               image[i][j - 1].rgbtBlue +
                               image[i][j].rgbtBlue +
                               image[i][j + 1].rgbtBlue +
                               image[i + 1][j - 1].rgbtBlue +
                               image[i + 1][j].rgbtBlue +
                               image[i + 1][j + 1].rgbtBlue
                              ) / 9.0);
                    // Evaluate the green component average over 3x3 grid
                    G = round((image[i - 1][j - 1].rgbtGreen +
                               image[i - 1][j].rgbtGreen +
                               image[i - 1][j + 1].rgbtGreen +
                               image[i][j - 1].rgbtGreen +
                               image[i][j].rgbtGreen +
                               image[i][j + 1].rgbtGreen +
                               image[i + 1][j - 1].rgbtGreen +
                               image[i + 1][j].rgbtGreen +
                               image[i + 1][j + 1].rgbtGreen
                              ) / 9.0);
                    // Evaluate the red component average over 3x3 grid
                    R = round((image[i - 1][j - 1].rgbtRed +
                               image[i - 1][j].rgbtRed +
                               image[i - 1][j + 1].rgbtRed +
                               image[i][j - 1].rgbtRed +
                               image[i][j].rgbtRed +
                               image[i][j + 1].rgbtRed +
                               image[i + 1][j - 1].rgbtRed +
                               image[i + 1][j].rgbtRed +
                               image[i + 1][j + 1].rgbtRed
                              ) / 9.0);
                    // Assigns the components value to the pixel on the temporary image
                    blurred[i][j].rgbtBlue = B;
                    blurred[i][j].rgbtGreen = G;
                    blurred[i][j].rgbtRed = R;
                }
                // Validate if the current pixel is not on the left edge
                else if (j == 0) // Marco izquierdo
                {
                    B = 0; 
                    G = 0; 
                    R = 0;
                    // Evaluate the blue component average over 3x2 grid
                    B = round((image[i - 1][j].rgbtBlue +
                               image[i - 1][j + 1].rgbtBlue +
                               image[i][j].rgbtBlue +
                               image[i][j + 1].rgbtBlue +
                               image[i + 1][j].rgbtBlue +
                               image[i + 1][j + 1].rgbtBlue
                              ) / 6.0);
                    // Evaluate the blue component average over 3x2 grid
                    G = round((image[i - 1][j].rgbtGreen +
                               image[i - 1][j + 1].rgbtGreen +
                               image[i][j].rgbtGreen +
                               image[i][j + 1].rgbtGreen +
                               image[i + 1][j].rgbtGreen +
                               image[i + 1][j + 1].rgbtGreen
                              ) / 6.0);
                    // Evaluate the red component average over 3x2 grid
                    R = round((image[i - 1][j].rgbtRed +
                               image[i - 1][j + 1].rgbtRed +
                               image[i][j].rgbtRed +
                               image[i][j + 1].rgbtRed +
                               image[i + 1][j].rgbtRed +
                               image[i + 1][j + 1].rgbtRed
                              ) / 6.0);
                    // Assigns the components value to the pixel on the temporary image
                    blurred[i][j].rgbtBlue = B;
                    blurred[i][j].rgbtGreen = G;
                    blurred[i][j].rgbtRed = R;
                }
                // Validate if the current pixel is not on the right edge
                else if (j == width - 1) // Marco derecho
                {
                    B = 0; 
                    G = 0; 
                    R = 0;
                    // Evaluate the blue component average over 3x2 grid
                    B = round((image[i - 1][j - 1].rgbtBlue +
                               image[i - 1][j].rgbtBlue +
                               image[i][j - 1].rgbtBlue +
                               image[i][j].rgbtBlue +
                               image[i + 1][j - 1].rgbtBlue +
                               image[i + 1][j].rgbtBlue
                              ) / 6.0);
                    // Evaluate the green component average over 3x2 grid
                    G = round((image[i - 1][j - 1].rgbtGreen +
                               image[i - 1][j].rgbtGreen +
                               image[i][j - 1].rgbtGreen +
                               image[i][j].rgbtGreen +
                               image[i + 1][j - 1].rgbtGreen +
                               image[i + 1][j].rgbtGreen
                              ) / 6.0);
                    // Evaluate the red component average over 3x2 grid
                    R = round((image[i - 1][j - 1].rgbtRed +
                               image[i - 1][j].rgbtRed +
                               image[i][j - 1].rgbtRed +
                               image[i][j].rgbtRed +
                               image[i + 1][j - 1].rgbtRed +
                               image[i + 1][j].rgbtRed
                              ) / 6.0);
                    // Assigns the components value to the pixel on the temporary image
                    blurred[i][j].rgbtBlue = B;
                    blurred[i][j].rgbtGreen = G;
                    blurred[i][j].rgbtRed = R;
                }
            }
            // Validate if the current pixel is on the top edge
            else if (i == 0)  // Marco superior
            {   
                // Validate if the current pixel is not on the left or right edges
                if (0 < j && j < width - 1)  // Centrales
                {
                    B = 0; 
                    G = 0; 
                    R = 0;
                    // Evaluate the blue component average over 2x3 grid
                    B = round((image[i][j - 1].rgbtBlue +
                               image[i][j].rgbtBlue +
                               image[i][j + 1].rgbtBlue +
                               image[i + 1][j - 1].rgbtBlue +
                               image[i + 1][j].rgbtBlue +
                               image[i + 1][j + 1].rgbtBlue
                              ) / 6.0);
                    // Evaluate the green component average over 2x3 grid
                    G = round((image[i][j - 1].rgbtGreen +
                               image[i][j].rgbtGreen +
                               image[i][j + 1].rgbtGreen +
                               image[i + 1][j - 1].rgbtGreen +
                               image[i + 1][j].rgbtGreen +
                               image[i + 1][j + 1].rgbtGreen
                              ) / 6.0);
                    // Evaluate the red component average over 2x3 grid
                    R = round((image[i][j - 1].rgbtRed +
                               image[i][j].rgbtRed +
                               image[i][j + 1].rgbtRed +
                               image[i + 1][j - 1].rgbtRed +
                               image[i + 1][j].rgbtRed +
                               image[i + 1][j + 1].rgbtRed
                              ) / 6.0);
                    // Assigns the components value to the pixel on the temporary image
                    blurred[i][j].rgbtBlue = B;
                    blurred[i][j].rgbtGreen = G;
                    blurred[i][j].rgbtRed = R;
                }
                // Validate if the current pixel is the top left corner
                else if (j == 0) // Esquina izquierda
                {
                    B = 0; 
                    G = 0; 
                    R = 0;
                    // Evaluate the blue component average over 2x2 grid
                    B = round((image[i][j].rgbtBlue +
                               image[i][j + 1].rgbtBlue +
                               image[i + 1][j].rgbtBlue +
                               image[i + 1][j + 1].rgbtBlue
                              ) / 4.0);
                    // Evaluate the green component average over 2x2 grid
                    G = round((image[i][j].rgbtGreen +
                               image[i][j + 1].rgbtGreen +
                               image[i + 1][j].rgbtGreen +
                               image[i + 1][j + 1].rgbtGreen
                              ) / 4.0);
                    // Evaluate the red component average over 2x2 grid
                    R = round((image[i][j].rgbtRed +
                               image[i][j + 1].rgbtRed +
                               image[i + 1][j].rgbtRed +
                               image[i + 1][j + 1].rgbtRed
                              ) / 4.0);
                    // Assigns the components value to the pixel on the temporary image
                    blurred[i][j].rgbtBlue = B;
                    blurred[i][j].rgbtGreen = G;
                    blurred[i][j].rgbtRed = R;
                }
                // Validate if the current pixel is the top right corner
                else if (j == width - 1)  // Esquina derecha
                {
                    B = 0; 
                    G = 0; 
                    R = 0;
                    // Evaluate the blue component average over 2x2 grid
                    B = round((image[i][j - 1].rgbtBlue +
                               image[i][j].rgbtBlue +
                               image[i + 1][j - 1].rgbtBlue +
                               image[i + 1][j].rgbtBlue
                              ) / 4.0);
                    // Evaluate the green component average over 2x2 grid
                    G = round((image[i][j - 1].rgbtGreen +
                               image[i][j].rgbtGreen +
                               image[i + 1][j - 1].rgbtGreen +
                               image[i + 1][j].rgbtGreen
                              ) / 4.0);
                    // Evaluate the red component average over 2x2 grid
                    R = round((image[i][j - 1].rgbtRed +
                               image[i][j].rgbtRed +
                               image[i + 1][j - 1].rgbtRed +
                               image[i + 1][j].rgbtRed
                              ) / 4.0);
                    // Assigns the components value to the pixel on the temporary image
                    blurred[i][j].rgbtBlue = B;
                    blurred[i][j].rgbtGreen = G;
                    blurred[i][j].rgbtRed = R;
                }
            }
            // Validate if the current pixel is on the bottom edge
            else if (i == height - 1)  // Marco inferior
            {
                // Validate if the current pixel is not on the left or right edges
                if (0 < j && j < width - 1)  // Centrales
                {
                    B = 0; 
                    G = 0; 
                    R = 0;
                    // Evaluate the blue component average over 2x3 grid
                    B = round((image[i - 1][j - 1].rgbtBlue +
                               image[i - 1][j].rgbtBlue +
                               image[i - 1][j + 1].rgbtBlue +
                               image[i][j - 1].rgbtBlue +
                               image[i][j].rgbtBlue +
                               image[i][j + 1].rgbtBlue
                              ) / 6.0);
                    // Evaluate the green component average over 2x3 grid
                    G = round((image[i - 1][j - 1].rgbtGreen +
                               image[i - 1][j].rgbtGreen +
                               image[i - 1][j + 1].rgbtGreen +
                               image[i][j - 1].rgbtGreen +
                               image[i][j].rgbtGreen +
                               image[i][j + 1].rgbtGreen
                              ) / 6.0);
                    // Evaluate the red component average over 2x3 grid
                    R = round((image[i - 1][j - 1].rgbtRed +
                               image[i - 1][j].rgbtRed +
                               image[i - 1][j + 1].rgbtRed +
                               image[i][j - 1].rgbtRed +
                               image[i][j].rgbtRed +
                               image[i][j + 1].rgbtRed
                              ) / 6.0);
                    // Assigns the components value to the pixel on the temporary image
                    blurred[i][j].rgbtBlue = B;
                    blurred[i][j].rgbtGreen = G;
                    blurred[i][j].rgbtRed = R;
                }
                // Validate if the current pixel is the bottom left corner
                else if (j == 0)  // Esquina izquierda
                {
                    B = 0; 
                    G = 0; 
                    R = 0;
                    // Evaluate the blue component average over 2x2 grid
                    B = round((image[i - 1][j].rgbtBlue +
                               image[i - 1][j + 1].rgbtBlue +
                               image[i][j].rgbtBlue +
                               image[i][j + 1].rgbtBlue
                              ) / 4.0);
                    // Evaluate the green component average over 2x2 grid
                    G = round((image[i - 1][j].rgbtGreen +
                               image[i - 1][j + 1].rgbtGreen +
                               image[i][j].rgbtGreen +
                               image[i][j + 1].rgbtGreen
                              ) / 4.0);
                    // Evaluate the red component average over 2x2 grid
                    R = round((image[i - 1][j].rgbtRed +
                               image[i - 1][j + 1].rgbtRed +
                               image[i][j].rgbtRed +
                               image[i][j + 1].rgbtRed
                              ) / 4.0);
                    // Assigns the components value to the pixel on the temporary image
                    blurred[i][j].rgbtBlue = B;
                    blurred[i][j].rgbtGreen = G;
                    blurred[i][j].rgbtRed = R;
                }
                // Validate if the current pixel is the bottom right corner
                else if (j == width - 1)  //Esquina derecha
                {
                    B = 0; 
                    G = 0; 
                    R = 0;
                    // Evaluate the blue component average over 2x2 grid
                    B = round((image[i - 1][j - 1].rgbtBlue +
                               image[i - 1][j].rgbtBlue +
                               image[i][j - 1].rgbtBlue +
                               image[i][j].rgbtBlue
                              ) / 4.0);
                    // Evaluate the green component average over 2x2 grid
                    G = round((image[i - 1][j - 1].rgbtGreen +
                               image[i - 1][j].rgbtGreen +
                               image[i][j - 1].rgbtGreen +
                               image[i][j].rgbtGreen
                              ) / 4.0);
                    // Evaluate the red component average over 2x2 grid
                    R = round((image[i - 1][j - 1].rgbtRed +
                               image[i - 1][j].rgbtRed +
                               image[i][j - 1].rgbtRed +
                               image[i][j].rgbtRed
                              ) / 4.0);
                    // Assigns the components value to the pixel on the temporary image
                    blurred[i][j].rgbtBlue = B;
                    blurred[i][j].rgbtGreen = G;
                    blurred[i][j].rgbtRed = R;
                }
            }
        }
    }
    // Assigns the new values to the image pointer
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = blurred[i][j];
        }
    }
    // Frees the alocated memory for the temporary image
    free(blurred);
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int R, G, B;
    float Gx, Gy;
    int ArrGx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int ArrGy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    RGBTRIPLE(*temp)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (temp == NULL)
    {
        fprintf(stderr, "Not enough memory to store blured image.\n");
        return;
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (0 < i && i < height - 1) // Centrales
            {
                if (0 < j && j < width - 1)  // Centrales
                {
                    B = 0; 
                    G = 0; 
                    R = 0; 
                    Gx = 0; 
                    Gy = 0;

                    Gx = (image[i - 1][j - 1].rgbtBlue * ArrGx[0][0])  +
                         (image[i - 1][j].rgbtBlue * ArrGx[0][1]) +
                         (image[i - 1][j + 1].rgbtBlue * ArrGx[0][2]) +
                         (image[i][j - 1].rgbtBlue * ArrGx[1][0]) +
                         (image[i][j].rgbtBlue * ArrGx[1][1]) +
                         (image[i][j + 1].rgbtBlue * ArrGx[1][2]) +
                         (image[i + 1][j - 1].rgbtBlue * ArrGx[2][0]) +
                         (image[i + 1][j].rgbtBlue * ArrGx[2][1]) +
                         (image[i + 1][j + 1].rgbtBlue * ArrGx[2][2]);

                    Gy = (image[i - 1][j - 1].rgbtBlue * ArrGy[0][0])  +
                         (image[i - 1][j].rgbtBlue * ArrGy[0][1]) +
                         (image[i - 1][j + 1].rgbtBlue * ArrGy[0][2]) +
                         (image[i][j - 1].rgbtBlue * ArrGy[1][0]) +
                         (image[i][j].rgbtBlue * ArrGy[1][1]) +
                         (image[i][j + 1].rgbtBlue * ArrGy[1][2]) +
                         (image[i + 1][j - 1].rgbtBlue * ArrGy[2][0]) +
                         (image[i + 1][j].rgbtBlue * ArrGy[2][1]) +
                         (image[i + 1][j + 1].rgbtBlue * ArrGy[2][2]);

                    B = tops(round(sqrt(pow(Gx, 2) + pow(Gy, 2))));

                    Gx = (image[i - 1][j - 1].rgbtGreen * ArrGx[0][0])  +
                         (image[i - 1][j].rgbtGreen * ArrGx[0][1]) +
                         (image[i - 1][j + 1].rgbtGreen * ArrGx[0][2]) +
                         (image[i][j - 1].rgbtGreen * ArrGx[1][0]) +
                         (image[i][j].rgbtGreen * ArrGx[1][1]) +
                         (image[i][j + 1].rgbtGreen * ArrGx[1][2]) +
                         (image[i + 1][j - 1].rgbtGreen * ArrGx[2][0]) +
                         (image[i + 1][j].rgbtGreen * ArrGx[2][1]) +
                         (image[i + 1][j + 1].rgbtGreen * ArrGx[2][2]);

                    Gy = (image[i - 1][j - 1].rgbtGreen * ArrGy[0][0])  +
                         (image[i - 1][j].rgbtGreen * ArrGy[0][1]) +
                         (image[i - 1][j + 1].rgbtGreen * ArrGy[0][2]) +
                         (image[i][j - 1].rgbtGreen * ArrGy[1][0]) +
                         (image[i][j].rgbtGreen * ArrGy[1][1]) +
                         (image[i][j + 1].rgbtGreen * ArrGy[1][2]) +
                         (image[i + 1][j - 1].rgbtGreen * ArrGy[2][0]) +
                         (image[i + 1][j].rgbtGreen * ArrGy[2][1]) +
                         (image[i + 1][j + 1].rgbtGreen * ArrGy[2][2]);

                    G = tops(round(sqrt(pow(Gx, 2) + pow(Gy, 2))));

                    Gx = (image[i - 1][j - 1].rgbtRed * ArrGx[0][0])  +
                         (image[i - 1][j].rgbtRed * ArrGx[0][1]) +
                         (image[i - 1][j + 1].rgbtRed * ArrGx[0][2]) +
                         (image[i][j - 1].rgbtRed * ArrGx[1][0]) +
                         (image[i][j].rgbtRed * ArrGx[1][1]) +
                         (image[i][j + 1].rgbtRed * ArrGx[1][2]) +
                         (image[i + 1][j - 1].rgbtRed * ArrGx[2][0]) +
                         (image[i + 1][j].rgbtRed * ArrGx[2][1]) +
                         (image[i + 1][j + 1].rgbtRed * ArrGx[2][2]);

                    Gy = (image[i - 1][j - 1].rgbtRed * ArrGy[0][0])  +
                         (image[i - 1][j].rgbtRed * ArrGy[0][1]) +
                         (image[i - 1][j + 1].rgbtRed * ArrGy[0][2]) +
                         (image[i][j - 1].rgbtRed * ArrGy[1][0]) +
                         (image[i][j].rgbtRed * ArrGy[1][1]) +
                         (image[i][j + 1].rgbtRed * ArrGy[1][2]) +
                         (image[i + 1][j - 1].rgbtRed * ArrGy[2][0]) +
                         (image[i + 1][j].rgbtRed * ArrGy[2][1]) +
                         (image[i + 1][j + 1].rgbtRed * ArrGy[2][2]);

                    R = tops(round(sqrt(pow(Gx, 2) + pow(Gy, 2))));

                    temp[i][j].rgbtBlue = B;
                    temp[i][j].rgbtGreen = G;
                    temp[i][j].rgbtRed = R;
                }
                else if (j == 0) // Marco izquierdo
                {
                    B = 0; 
                    G = 0; 
                    R = 0; 
                    Gx = 0; 
                    Gy = 0;

                    Gx = (image[i - 1][j].rgbtBlue * ArrGx[0][1]) +
                         (image[i - 1][j + 1].rgbtBlue * ArrGx[0][2]) +
                         (image[i][j].rgbtBlue * ArrGx[1][1]) +
                         (image[i][j + 1].rgbtBlue * ArrGx[1][2]) +
                         (image[i + 1][j].rgbtBlue * ArrGx[2][1]) +
                         (image[i + 1][j + 1].rgbtBlue * ArrGx[2][2]);

                    Gy = (image[i - 1][j].rgbtBlue * ArrGy[0][1]) +
                         (image[i - 1][j + 1].rgbtBlue * ArrGy[0][2]) +
                         (image[i][j].rgbtBlue * ArrGy[1][1]) +
                         (image[i][j + 1].rgbtBlue * ArrGy[1][2]) +
                         (image[i + 1][j].rgbtBlue * ArrGy[2][1]) +
                         (image[i + 1][j + 1].rgbtBlue * ArrGy[2][2]);

                    B = tops(round(sqrt(pow(Gx, 2) + pow(Gy, 2))));

                    Gx = (image[i - 1][j].rgbtGreen * ArrGx[0][1]) +
                         (image[i - 1][j + 1].rgbtGreen * ArrGx[0][2]) +
                         (image[i][j].rgbtGreen * ArrGx[1][1]) +
                         (image[i][j + 1].rgbtGreen * ArrGx[1][2]) +
                         (image[i + 1][j].rgbtGreen * ArrGx[2][1]) +
                         (image[i + 1][j + 1].rgbtGreen * ArrGx[2][2]);

                    Gy = (image[i - 1][j].rgbtGreen * ArrGy[0][1]) +
                         (image[i - 1][j + 1].rgbtGreen * ArrGy[0][2]) +
                         (image[i][j].rgbtGreen * ArrGy[1][1]) +
                         (image[i][j + 1].rgbtGreen * ArrGy[1][2]) +
                         (image[i + 1][j].rgbtGreen * ArrGy[2][1]) +
                         (image[i + 1][j + 1].rgbtGreen * ArrGy[2][2]);

                    G = tops(round(sqrt(pow(Gx, 2) + pow(Gy, 2))));

                    Gx = (image[i - 1][j].rgbtRed * ArrGx[0][1]) +
                         (image[i - 1][j + 1].rgbtRed * ArrGx[0][2]) +
                         (image[i][j].rgbtRed * ArrGx[1][1]) +
                         (image[i][j + 1].rgbtRed * ArrGx[1][2]) +
                         (image[i + 1][j].rgbtRed * ArrGx[2][1]) +
                         (image[i + 1][j + 1].rgbtRed * ArrGx[2][2]);

                    Gy = (image[i - 1][j].rgbtRed * ArrGy[0][1]) +
                         (image[i - 1][j + 1].rgbtRed * ArrGy[0][2]) +
                         (image[i][j].rgbtRed * ArrGy[1][1]) +
                         (image[i][j + 1].rgbtRed * ArrGy[1][2]) +
                         (image[i + 1][j].rgbtRed * ArrGy[2][1]) +
                         (image[i + 1][j + 1].rgbtRed * ArrGy[2][2]);

                    R = tops(round(sqrt(pow(Gx, 2) + pow(Gy, 2))));

                    temp[i][j].rgbtBlue = B;
                    temp[i][j].rgbtGreen = G;
                    temp[i][j].rgbtRed = R;
                }
                else if (j == width - 1) // Marco derecho
                {
                    B = 0; 
                    G = 0; 
                    R = 0; 
                    Gx = 0; 
                    Gy = 0;

                    Gx = (image[i - 1][j - 1].rgbtBlue * ArrGx[0][0])  +
                         (image[i - 1][j].rgbtBlue * ArrGx[0][1]) +
                         (image[i][j - 1].rgbtBlue * ArrGx[1][0]) +
                         (image[i][j].rgbtBlue * ArrGx[1][1]) +
                         (image[i + 1][j - 1].rgbtBlue * ArrGx[2][0]) +
                         (image[i + 1][j].rgbtBlue * ArrGx[2][1]);

                    Gy = (image[i - 1][j - 1].rgbtBlue * ArrGy[0][0])  +
                         (image[i - 1][j].rgbtBlue * ArrGy[0][1]) +
                         (image[i][j - 1].rgbtBlue * ArrGy[1][0]) +
                         (image[i][j].rgbtBlue * ArrGy[1][1]) +
                         (image[i + 1][j - 1].rgbtBlue * ArrGy[2][0]) +
                         (image[i + 1][j].rgbtBlue * ArrGy[2][1]);

                    B = tops(round(sqrt(pow(Gx, 2) + pow(Gy, 2))));

                    Gx = (image[i - 1][j - 1].rgbtGreen * ArrGx[0][0])  +
                         (image[i - 1][j].rgbtGreen * ArrGx[0][1]) +
                         (image[i][j - 1].rgbtGreen * ArrGx[1][0]) +
                         (image[i][j].rgbtGreen * ArrGx[1][1]) +
                         (image[i + 1][j - 1].rgbtGreen * ArrGx[2][0]) +
                         (image[i + 1][j].rgbtGreen * ArrGx[2][1]);

                    Gy = (image[i - 1][j - 1].rgbtGreen * ArrGy[0][0])  +
                         (image[i - 1][j].rgbtGreen * ArrGy[0][1]) +
                         (image[i][j - 1].rgbtGreen * ArrGy[1][0]) +
                         (image[i][j].rgbtGreen * ArrGy[1][1]) +
                         (image[i + 1][j - 1].rgbtGreen * ArrGy[2][0]) +
                         (image[i + 1][j].rgbtGreen * ArrGy[2][1]);

                    G = tops(round(sqrt(pow(Gx, 2) + pow(Gy, 2))));

                    Gx = (image[i - 1][j - 1].rgbtRed * ArrGx[0][0])  +
                         (image[i - 1][j].rgbtRed * ArrGx[0][1]) +
                         (image[i][j - 1].rgbtRed * ArrGx[1][0]) +
                         (image[i][j].rgbtRed * ArrGx[1][1]) +
                         (image[i + 1][j - 1].rgbtRed * ArrGx[2][0]) +
                         (image[i + 1][j].rgbtRed * ArrGx[2][1]);

                    Gy = (image[i - 1][j - 1].rgbtRed * ArrGy[0][0])  +
                         (image[i - 1][j].rgbtRed * ArrGy[0][1]) +
                         (image[i][j - 1].rgbtRed * ArrGy[1][0]) +
                         (image[i][j].rgbtRed * ArrGy[1][1]) +
                         (image[i + 1][j - 1].rgbtRed * ArrGy[2][0]) +
                         (image[i + 1][j].rgbtRed * ArrGy[2][1]);

                    R = tops(round(sqrt(pow(Gx, 2) + pow(Gy, 2))));

                    temp[i][j].rgbtBlue = B;
                    temp[i][j].rgbtGreen = G;
                    temp[i][j].rgbtRed = R;
                }
            }
            else if (i == 0)  // Marco superior
            {
                if (0 < j && j < width - 1)  // Centrales
                {
                    B = 0; 
                    G = 0; 
                    R = 0; 
                    Gx = 0; 
                    Gy = 0;

                    Gx = (image[i][j - 1].rgbtBlue * ArrGx[1][0]) +
                         (image[i][j].rgbtBlue * ArrGx[1][1]) +
                         (image[i][j + 1].rgbtBlue * ArrGx[1][2]) +
                         (image[i + 1][j - 1].rgbtBlue * ArrGx[2][0]) +
                         (image[i + 1][j].rgbtBlue * ArrGx[2][1]) +
                         (image[i + 1][j + 1].rgbtBlue * ArrGx[2][2]);

                    Gy = (image[i][j - 1].rgbtBlue * ArrGy[1][0]) +
                         (image[i][j].rgbtBlue * ArrGy[1][1]) +
                         (image[i][j + 1].rgbtBlue * ArrGy[1][2]) +
                         (image[i + 1][j - 1].rgbtBlue * ArrGy[2][0]) +
                         (image[i + 1][j].rgbtBlue * ArrGy[2][1]) +
                         (image[i + 1][j + 1].rgbtBlue * ArrGy[2][2]);

                    B = tops(round(sqrt(pow(Gx, 2) + pow(Gy, 2))));

                    Gx = (image[i][j - 1].rgbtGreen * ArrGx[1][0]) +
                         (image[i][j].rgbtGreen * ArrGx[1][1]) +
                         (image[i][j + 1].rgbtGreen * ArrGx[1][2]) +
                         (image[i + 1][j - 1].rgbtGreen * ArrGx[2][0]) +
                         (image[i + 1][j].rgbtGreen * ArrGx[2][1]) +
                         (image[i + 1][j + 1].rgbtGreen * ArrGx[2][2]);

                    Gy = (image[i][j - 1].rgbtGreen * ArrGy[1][0]) +
                         (image[i][j].rgbtGreen * ArrGy[1][1]) +
                         (image[i][j + 1].rgbtGreen * ArrGy[1][2]) +
                         (image[i + 1][j - 1].rgbtGreen * ArrGy[2][0]) +
                         (image[i + 1][j].rgbtGreen * ArrGy[2][1]) +
                         (image[i + 1][j + 1].rgbtGreen * ArrGy[2][2]);

                    G = tops(round(sqrt(pow(Gx, 2) + pow(Gy, 2))));

                    Gx = (image[i][j - 1].rgbtRed * ArrGx[1][0]) +
                         (image[i][j].rgbtRed * ArrGx[1][1]) +
                         (image[i][j + 1].rgbtRed * ArrGx[1][2]) +
                         (image[i + 1][j - 1].rgbtRed * ArrGx[2][0]) +
                         (image[i + 1][j].rgbtRed * ArrGx[2][1]) +
                         (image[i + 1][j + 1].rgbtRed * ArrGx[2][2]);

                    Gy = (image[i][j - 1].rgbtRed * ArrGy[1][0]) +
                         (image[i][j].rgbtRed * ArrGy[1][1]) +
                         (image[i][j + 1].rgbtRed * ArrGy[1][2]) +
                         (image[i + 1][j - 1].rgbtRed * ArrGy[2][0]) +
                         (image[i + 1][j].rgbtRed * ArrGy[2][1]) +
                         (image[i + 1][j + 1].rgbtRed * ArrGy[2][2]);

                    R = tops(round(sqrt(pow(Gx, 2) + pow(Gy, 2))));

                    temp[i][j].rgbtBlue = B;
                    temp[i][j].rgbtGreen = G;
                    temp[i][j].rgbtRed = R;
                }
                else if (j == 0) // Esquina izquierda
                {
                    B = 0; 
                    G = 0; 
                    R = 0; 
                    Gx = 0; 
                    Gy = 0;

                    Gx = (image[i][j].rgbtBlue * ArrGx[1][1]) +
                         (image[i][j + 1].rgbtBlue * ArrGx[1][2]) +
                         (image[i + 1][j].rgbtBlue * ArrGx[2][1]) +
                         (image[i + 1][j + 1].rgbtBlue * ArrGx[2][2]);

                    Gy = (image[i][j].rgbtBlue * ArrGy[1][1]) +
                         (image[i][j + 1].rgbtBlue * ArrGy[1][2]) +
                         (image[i + 1][j].rgbtBlue * ArrGy[2][1]) +
                         (image[i + 1][j + 1].rgbtBlue * ArrGy[2][2]);

                    B = tops(round(sqrt(pow(Gx, 2) + pow(Gy, 2))));

                    Gx = (image[i][j].rgbtGreen * ArrGx[1][1]) +
                         (image[i][j + 1].rgbtGreen * ArrGx[1][2]) +
                         (image[i + 1][j].rgbtGreen * ArrGx[2][1]) +
                         (image[i + 1][j + 1].rgbtGreen * ArrGx[2][2]);

                    Gy = (image[i][j].rgbtGreen * ArrGy[1][1]) +
                         (image[i][j + 1].rgbtGreen * ArrGy[1][2]) +
                         (image[i + 1][j].rgbtGreen * ArrGy[2][1]) +
                         (image[i + 1][j + 1].rgbtGreen * ArrGy[2][2]);

                    G = tops(round(sqrt(pow(Gx, 2) + pow(Gy, 2))));

                    Gx = (image[i][j].rgbtRed * ArrGx[1][1]) +
                         (image[i][j + 1].rgbtRed * ArrGx[1][2]) +
                         (image[i + 1][j].rgbtRed * ArrGx[2][1]) +
                         (image[i + 1][j + 1].rgbtRed * ArrGx[2][2]);

                    Gy = (image[i][j].rgbtRed * ArrGy[1][1]) +
                         (image[i][j + 1].rgbtRed * ArrGy[1][2]) +
                         (image[i + 1][j].rgbtRed * ArrGy[2][1]) +
                         (image[i + 1][j + 1].rgbtRed * ArrGy[2][2]);

                    R = tops(round(sqrt(pow(Gx, 2) + pow(Gy, 2))));

                    temp[i][j].rgbtBlue = B;
                    temp[i][j].rgbtGreen = G;
                    temp[i][j].rgbtRed = R;
                }
                else if (j == width - 1)  // Esquina derecha
                {
                    B = 0; 
                    G = 0; 
                    R = 0; 
                    Gx = 0; 
                    Gy = 0;

                    Gx = (image[i][j - 1].rgbtBlue * ArrGx[1][0]) +
                         (image[i][j].rgbtBlue * ArrGx[1][1]) +
                         (image[i + 1][j - 1].rgbtBlue * ArrGx[2][0]) +
                         (image[i + 1][j].rgbtBlue * ArrGx[2][1]);

                    Gy = (image[i][j - 1].rgbtBlue * ArrGy[1][0]) +
                         (image[i][j].rgbtBlue * ArrGy[1][1]) +
                         (image[i + 1][j - 1].rgbtBlue * ArrGy[2][0]) +
                         (image[i + 1][j].rgbtBlue * ArrGy[2][1]);

                    B = tops(round(sqrt(pow(Gx, 2) + pow(Gy, 2))));

                    Gx = (image[i][j - 1].rgbtGreen * ArrGx[1][0]) +
                         (image[i][j].rgbtGreen * ArrGx[1][1]) +
                         (image[i + 1][j - 1].rgbtGreen * ArrGx[2][0]) +
                         (image[i + 1][j].rgbtGreen * ArrGx[2][1]);

                    Gy = (image[i][j - 1].rgbtGreen * ArrGy[1][0]) +
                         (image[i][j].rgbtGreen * ArrGy[1][1]) +
                         (image[i + 1][j - 1].rgbtGreen * ArrGy[2][0]) +
                         (image[i + 1][j].rgbtGreen * ArrGy[2][1]);

                    G = tops(round(sqrt(pow(Gx, 2) + pow(Gy, 2))));

                    Gx = (image[i][j - 1].rgbtRed * ArrGx[1][0]) +
                         (image[i][j].rgbtRed * ArrGx[1][1]) +
                         (image[i + 1][j - 1].rgbtRed * ArrGx[2][0]) +
                         (image[i + 1][j].rgbtRed * ArrGx[2][1]);

                    Gy = (image[i][j - 1].rgbtRed * ArrGy[1][0]) +
                         (image[i][j].rgbtRed * ArrGy[1][1]) +
                         (image[i + 1][j - 1].rgbtRed * ArrGy[2][0]) +
                         (image[i + 1][j].rgbtRed * ArrGy[2][1]);

                    R = tops(round(sqrt(pow(Gx, 2) + pow(Gy, 2))));

                    temp[i][j].rgbtBlue = B;
                    temp[i][j].rgbtGreen = G;
                    temp[i][j].rgbtRed = R;
                }
            }
            else if (i == height - 1)  // Marco inferior
            {
                if (0 < j && j < width - 1)  // Centrales
                {
                    B = 0; 
                    G = 0; 
                    R = 0; 
                    Gx = 0; 
                    Gy = 0;

                    Gx = (image[i - 1][j - 1].rgbtBlue * ArrGx[0][0])  +
                         (image[i - 1][j].rgbtBlue * ArrGx[0][1]) +
                         (image[i - 1][j + 1].rgbtBlue * ArrGx[0][2]) +
                         (image[i][j - 1].rgbtBlue * ArrGx[1][0]) +
                         (image[i][j].rgbtBlue * ArrGx[1][1]) +
                         (image[i][j + 1].rgbtBlue * ArrGx[1][2]);

                    Gy = (image[i - 1][j - 1].rgbtBlue * ArrGy[0][0])  +
                         (image[i - 1][j].rgbtBlue * ArrGy[0][1]) +
                         (image[i - 1][j + 1].rgbtBlue * ArrGy[0][2]) +
                         (image[i][j - 1].rgbtBlue * ArrGy[1][0]) +
                         (image[i][j].rgbtBlue * ArrGy[1][1]) +
                         (image[i][j + 1].rgbtBlue * ArrGy[1][2]);

                    B = tops(round(sqrt(pow(Gx, 2) + pow(Gy, 2))));

                    Gx = (image[i - 1][j - 1].rgbtGreen * ArrGx[0][0])  +
                         (image[i - 1][j].rgbtGreen * ArrGx[0][1]) +
                         (image[i - 1][j + 1].rgbtGreen * ArrGx[0][2]) +
                         (image[i][j - 1].rgbtGreen * ArrGx[1][0]) +
                         (image[i][j].rgbtGreen * ArrGx[1][1]) +
                         (image[i][j + 1].rgbtGreen * ArrGx[1][2]);

                    Gy = (image[i - 1][j - 1].rgbtGreen * ArrGy[0][0])  +
                         (image[i - 1][j].rgbtGreen * ArrGy[0][1]) +
                         (image[i - 1][j + 1].rgbtGreen * ArrGy[0][2]) +
                         (image[i][j - 1].rgbtGreen * ArrGy[1][0]) +
                         (image[i][j].rgbtGreen * ArrGy[1][1]) +
                         (image[i][j + 1].rgbtGreen * ArrGy[1][2]);

                    G = tops(round(sqrt(pow(Gx, 2) + pow(Gy, 2))));

                    Gx = (image[i - 1][j - 1].rgbtRed * ArrGx[0][0])  +
                         (image[i - 1][j].rgbtRed * ArrGx[0][1]) +
                         (image[i - 1][j + 1].rgbtRed * ArrGx[0][2]) +
                         (image[i][j - 1].rgbtRed * ArrGx[1][0]) +
                         (image[i][j].rgbtRed * ArrGx[1][1]) +
                         (image[i][j + 1].rgbtRed * ArrGx[1][2]);

                    Gy = (image[i - 1][j - 1].rgbtRed * ArrGy[0][0])  +
                         (image[i - 1][j].rgbtRed * ArrGy[0][1]) +
                         (image[i - 1][j + 1].rgbtRed * ArrGy[0][2]) +
                         (image[i][j - 1].rgbtRed * ArrGy[1][0]) +
                         (image[i][j].rgbtRed * ArrGy[1][1]) +
                         (image[i][j + 1].rgbtRed * ArrGy[1][2]);

                    R = tops(round(sqrt(pow(Gx, 2) + pow(Gy, 2))));

                    temp[i][j].rgbtBlue = B;
                    temp[i][j].rgbtGreen = G;
                    temp[i][j].rgbtRed = R;
                }
                else if (j == 0)  // Esquina izquierda
                {
                    B = 0; 
                    G = 0; 
                    R = 0; 
                    Gx = 0; 
                    Gy = 0;

                    Gx = (image[i - 1][j].rgbtBlue * ArrGx[0][1]) +
                         (image[i - 1][j + 1].rgbtBlue * ArrGx[0][2]) +
                         (image[i][j].rgbtBlue * ArrGx[1][1]) +
                         (image[i][j + 1].rgbtBlue * ArrGx[1][2]);

                    Gy = (image[i - 1][j].rgbtBlue * ArrGy[0][1]) +
                         (image[i - 1][j + 1].rgbtBlue * ArrGy[0][2]) +
                         (image[i][j].rgbtBlue * ArrGy[1][1]) +
                         (image[i][j + 1].rgbtBlue * ArrGy[1][2]);

                    B = tops(round(sqrt(pow(Gx, 2) + pow(Gy, 2))));

                    Gx = (image[i - 1][j].rgbtGreen * ArrGx[0][1]) +
                         (image[i - 1][j + 1].rgbtGreen * ArrGx[0][2]) +
                         (image[i][j].rgbtGreen * ArrGx[1][1]) +
                         (image[i][j + 1].rgbtGreen * ArrGx[1][2]);

                    Gy = (image[i - 1][j].rgbtGreen * ArrGy[0][1]) +
                         (image[i - 1][j + 1].rgbtGreen * ArrGy[0][2]) +
                         (image[i][j].rgbtGreen * ArrGy[1][1]) +
                         (image[i][j + 1].rgbtGreen * ArrGy[1][2]);

                    G = tops(round(sqrt(pow(Gx, 2) + pow(Gy, 2))));
     
                    Gx = (image[i - 1][j].rgbtRed * ArrGx[0][1]) +
                         (image[i - 1][j + 1].rgbtRed * ArrGx[0][2]) +
                         (image[i][j].rgbtRed * ArrGx[1][1]) +
                         (image[i][j + 1].rgbtRed * ArrGx[1][2]);

                    Gy = (image[i - 1][j].rgbtRed * ArrGy[0][1]) +
                         (image[i - 1][j + 1].rgbtRed * ArrGy[0][2]) +
                         (image[i][j].rgbtRed * ArrGy[1][1]) +
                         (image[i][j + 1].rgbtRed * ArrGy[1][2]);

                    R = tops(round(sqrt(pow(Gx, 2) + pow(Gy, 2))));

                    temp[i][j].rgbtBlue = B;
                    temp[i][j].rgbtGreen = G;
                    temp[i][j].rgbtRed = R;
                }
                else if (j == width - 1)  //Esquina derecha
                {
                    B = 0; 
                    G = 0; 
                    R = 0; 
                    Gx = 0; 
                    Gy = 0;

                    Gx = (image[i - 1][j - 1].rgbtBlue * ArrGx[0][0])  +
                         (image[i][j - 1].rgbtBlue * ArrGx[1][0]) +
                         (image[i][j].rgbtBlue * ArrGy[1][1]);

                    Gy = (image[i - 1][j - 1].rgbtBlue * ArrGy[0][0])  +
                         (image[i - 1][j].rgbtBlue * ArrGy[0][1]) +
                         (image[i][j - 1].rgbtBlue * ArrGy[1][0]) +
                         (image[i][j].rgbtBlue * ArrGy[1][1]);

                    B = tops(round(sqrt(pow(Gx, 2) + pow(Gy, 2))));

                    Gx = (image[i - 1][j - 1].rgbtGreen * ArrGx[0][0])  +
                         (image[i - 1][j].rgbtGreen * ArrGx[0][1]) +
                         (image[i][j - 1].rgbtGreen * ArrGx[1][0]) +
                         (image[i][j].rgbtGreen * ArrGx[1][1]);

                    Gy = (image[i - 1][j - 1].rgbtGreen * ArrGy[0][0])  +
                         (image[i - 1][j].rgbtGreen * ArrGy[0][1]) +
                         (image[i][j - 1].rgbtGreen * ArrGy[1][0]) +
                         (image[i][j].rgbtGreen * ArrGy[1][1]);

                    G = tops(round(sqrt(pow(Gx, 2) + pow(Gy, 2))));

                    Gx = (image[i - 1][j - 1].rgbtRed * ArrGx[0][0])  +
                         (image[i - 1][j].rgbtRed * ArrGx[0][1]) +
                         (image[i][j - 1].rgbtRed * ArrGx[1][0]) +
                         (image[i][j].rgbtRed * ArrGx[1][1]);

                    Gy = (image[i - 1][j - 1].rgbtRed * ArrGy[0][0])  +
                         (image[i - 1][j].rgbtRed * ArrGy[0][1]) +
                         (image[i][j - 1].rgbtRed * ArrGy[1][0]) +
                         (image[i][j].rgbtRed * ArrGy[1][1]);

                    R = tops(round(sqrt(pow(Gx, 2) + pow(Gy, 2))));

                    temp[i][j].rgbtBlue = B;
                    temp[i][j].rgbtGreen = G;
                    temp[i][j].rgbtRed = R;
                }
            }
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
    free(temp);
    return;
}

