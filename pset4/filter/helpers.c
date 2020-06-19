#include "helpers.h"
#include <math.h>


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int greyscaleValue = round(((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0));
            image[i][j].rgbtBlue = greyscaleValue;
            image[i][j].rgbtGreen = greyscaleValue;
            image[i][j].rgbtRed = greyscaleValue;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sepiaRed = round((0.393 * image[i][j].rgbtRed) + (0.769 * image[i][j].rgbtGreen) + (0.189 * image[i][j].rgbtBlue));
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            int sepiaGreen = round((0.349 * image[i][j].rgbtRed) + (0.686 * image[i][j].rgbtGreen) + (0.168 * image[i][j].rgbtBlue));
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            int sepiaBlue = round((0.272 * image[i][j].rgbtRed) + (0.534 * image[i][j].rgbtGreen) + (0.131 * image[i][j].rgbtBlue));
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }
            image[i][j].rgbtBlue = sepiaBlue;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtRed = sepiaRed;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy to replace values with.
    RGBTRIPLE imageReflect[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            imageReflect[i][j] = image[i][width - j - 1];
        }
    }

    // Copy the reflected image onto image.
    for (int a = 0; a < height; a++)
    {
        for (int b = 0; b < width; b++)
        {
            image[a][b] = imageReflect[a][b];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    // Create a copy to replace values with.
    RGBTRIPLE imageBlur[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // If pixel is on top left corner.
            if (i == 0 && j == 0)
            {
                int blurValueBlue = round((image[i][j].rgbtBlue + image[i+1][j].rgbtBlue + image[i][j+1].rgbtBlue + image[i+1][j+1].rgbtBlue) / 4.0);
                int blurValueGreen = round((image[i][j].rgbtGreen + image[i+1][j].rgbtGreen + image[i][j+1].rgbtGreen + image[i+1][j+1].rgbtGreen) / 4.0);
                int blurValueRed = round((image[i][j].rgbtRed + image[i+1][j].rgbtRed + image[i][j+1].rgbtRed + image[i+1][j+1].rgbtRed) / 4.0);
                imageBlur[i][j].rgbtBlue = blurValueBlue;
                imageBlur[i][j].rgbtGreen = blurValueGreen;
                imageBlur[i][j].rgbtRed = blurValueRed;
            }
            //If the pixel is on the top right corner.
            else if (i == 0 && j == width - 1)
            {
                int blurValueBlue = round((image[i][j].rgbtBlue + image[i+1][j].rgbtBlue + image[i][j-1].rgbtBlue + image[i+1][j-1].rgbtBlue) / 4.0);
                int blurValueGreen = round((image[i][j].rgbtGreen + image[i+1][j].rgbtGreen + image[i][j-1].rgbtGreen + image[i+1][j-1].rgbtGreen) / 4.0);
                int blurValueRed = round((image[i][j].rgbtRed + image[i+1][j].rgbtRed + image[i][j-1].rgbtRed + image[i+1][j-1].rgbtRed) / 4.0);
                imageBlur[i][j].rgbtBlue = blurValueBlue;
                imageBlur[i][j].rgbtGreen = blurValueGreen;
                imageBlur[i][j].rgbtRed = blurValueRed;
            }
            //If the pixel is on the bottom left corner.
            else if (i == height - 1 && j == 0)
            {
                int blurValueBlue = round((image[i][j].rgbtBlue + image[i-1][j].rgbtBlue + image[i][j+1].rgbtBlue + image[i-1][j+1].rgbtBlue) / 4.0);
                int blurValueGreen = round((image[i][j].rgbtGreen + image[i-1][j].rgbtGreen + image[i][j+1].rgbtGreen + image[i-1][j+1].rgbtGreen) / 4.0);
                int blurValueRed = round((image[i][j].rgbtRed + image[i-1][j].rgbtRed + image[i][j+1].rgbtRed + image[i-1][j+1].rgbtRed) / 4.0);
                imageBlur[i][j].rgbtBlue = blurValueBlue;
                imageBlur[i][j].rgbtGreen = blurValueGreen;
                imageBlur[i][j].rgbtRed = blurValueRed;
            }
            // If the pixel is on the bottom right corner.
            else if (i == height - 1 && j == width - 1)
            {
                int blurValueBlue = round((image[i][j].rgbtBlue + image[i-1][j].rgbtBlue + image[i][j-1].rgbtBlue + image[i-1][j-1].rgbtBlue) / 4.0);
                int blurValueGreen = round((image[i][j].rgbtGreen + image[i-1][j].rgbtGreen + image[i][j-1].rgbtGreen + image[i-1][j-1].rgbtGreen) / 4.0);
                int blurValueRed = round((image[i][j].rgbtRed + image[i-1][j].rgbtRed + image[i][j-1].rgbtRed + image[i-1][j-1].rgbtRed) / 4.0);
                imageBlur[i][j].rgbtBlue = blurValueBlue;
                imageBlur[i][j].rgbtGreen = blurValueGreen;
                imageBlur[i][j].rgbtRed = blurValueRed;
            }
            // If the pixel is along the top border.
            else if (i == 0)
            {
                int blurValueBlue = round((image[i][j].rgbtBlue + image[i][j-1].rgbtBlue + image[i+1][j-1].rgbtBlue + image[i+1][j].rgbtBlue + image[i+1][j+1].rgbtBlue + image[i][j+1].rgbtBlue) / 6.0);
                int blurValueGreen = round((image[i][j].rgbtGreen + image[i][j-1].rgbtGreen + image[i+1][j-1].rgbtGreen + image[i+1][j].rgbtGreen + image[i+1][j+1].rgbtGreen + image[i][j+1].rgbtGreen) / 6.0);
                int blurValueRed = round((image[i][j].rgbtRed + image[i][j-1].rgbtRed + image[i+1][j-1].rgbtRed + image[i+1][j].rgbtRed + image[i+1][j+1].rgbtRed + image[i][j+1].rgbtRed) / 6.0);
                imageBlur[i][j].rgbtBlue = blurValueBlue;
                imageBlur[i][j].rgbtGreen = blurValueGreen;
                imageBlur[i][j].rgbtRed = blurValueRed;
            }
            // If the pixel is along the left border.
            else if (j == 0)
            {
                int blurValueBlue = round((image[i][j].rgbtBlue + image[i-1][j].rgbtBlue + image[i-1][j+1].rgbtBlue + image[i][j+1].rgbtBlue + image[i+1][j+1].rgbtBlue + image[i+1][j].rgbtBlue) / 6.0);
                int blurValueGreen = round((image[i][j].rgbtGreen + image[i-1][j].rgbtGreen + image[i-1][j+1].rgbtGreen + image[i][j+1].rgbtGreen + image[i+1][j+1].rgbtGreen + image[i+1][j].rgbtGreen) / 6.0);
                int blurValueRed = round((image[i][j].rgbtRed + image[i-1][j].rgbtRed + image[i-1][j+1].rgbtRed + image[i][j+1].rgbtRed + image[i+1][j+1].rgbtRed + image[i+1][j].rgbtRed) / 6.0);
                imageBlur[i][j].rgbtBlue = blurValueBlue;
                imageBlur[i][j].rgbtGreen = blurValueGreen;
                imageBlur[i][j].rgbtRed = blurValueRed;
            }
            // If the pixel is along the right border.
            else if (j == width - 1)
            {
                int blurValueBlue = round((image[i][j].rgbtBlue + image[i-1][j].rgbtBlue + image[i-1][j-1].rgbtBlue + image[i][j-1].rgbtBlue + image[i+1][j-1].rgbtBlue + image[i+1][j].rgbtBlue) / 6.0);
                int blurValueGreen = round((image[i][j].rgbtGreen + image[i-1][j].rgbtGreen + image[i-1][j-1].rgbtGreen + image[i][j-1].rgbtGreen + image[i+1][j-1].rgbtGreen + image[i+1][j].rgbtGreen) / 6.0);
                int blurValueRed = round((image[i][j].rgbtRed + image[i-1][j].rgbtRed + image[i-1][j-1].rgbtRed + image[i][j-1].rgbtRed + image[i+1][j-1].rgbtRed + image[i+1][j].rgbtRed) / 6.0);
                imageBlur[i][j].rgbtBlue = blurValueBlue;
                imageBlur[i][j].rgbtGreen = blurValueGreen;
                imageBlur[i][j].rgbtRed = blurValueRed;
            }
            // If the pixel is along the bottom border.
            else if (i == height - 1)
            {
                int blurValueBlue = round((image[i][j].rgbtBlue + image[i][j+1].rgbtBlue + image[i-1][j+1].rgbtBlue + image[i-1][j].rgbtBlue + image[i-1][j-1].rgbtBlue + image[i][j-1].rgbtBlue) / 6.0);
                int blurValueGreen = round((image[i][j].rgbtGreen + image[i][j+1].rgbtGreen + image[i-1][j+1].rgbtGreen + image[i-1][j].rgbtGreen + image[i-1][j-1].rgbtGreen + image[i][j-1].rgbtGreen) / 6.0);
                int blurValueRed = round((image[i][j].rgbtRed + image[i][j+1].rgbtRed + image[i-1][j+1].rgbtRed + image[i-1][j].rgbtRed + image[i-1][j-1].rgbtRed + image[i][j-1].rgbtRed) / 6.0);
                imageBlur[i][j].rgbtBlue = blurValueBlue;
                imageBlur[i][j].rgbtGreen = blurValueGreen;
                imageBlur[i][j].rgbtRed = blurValueRed;
            }
            // if pixel is somewhere in the middle surrounded by box.
            else
            {
                int blurValueBlue = round((image[i][j].rgbtBlue + image[i-1][j].rgbtBlue + image[i-1][j-1].rgbtBlue + image[i][j-1].rgbtBlue + image[i+1][j-1].rgbtBlue + image[i+1][j].rgbtBlue + image[i+1][j+1].rgbtBlue + image[i][j+1].rgbtBlue + image[i-1][j+1].rgbtBlue) / 9.0);
                int blurValueGreen = round((image[i][j].rgbtGreen + image[i-1][j].rgbtGreen + image[i-1][j-1].rgbtGreen + image[i][j-1].rgbtGreen + image[i+1][j-1].rgbtGreen + image[i+1][j].rgbtGreen + image[i+1][j+1].rgbtGreen + image[i][j+1].rgbtGreen + image[i-1][j+1].rgbtGreen) / 9.0);
                int blurValueRed = round((image[i][j].rgbtRed + image[i-1][j].rgbtRed + image[i-1][j-1].rgbtRed + image[i][j-1].rgbtRed + image[i+1][j-1].rgbtRed + image[i+1][j].rgbtRed + image[i+1][j+1].rgbtRed + image[i][j+1].rgbtRed + image[i-1][j+1].rgbtRed) / 9.0);
                imageBlur[i][j].rgbtBlue = blurValueBlue;
                imageBlur[i][j].rgbtGreen = blurValueGreen;
                imageBlur[i][j].rgbtRed = blurValueRed;
            }
        }
    }

    // Copy the blurred image onto image.
    for (int a = 0; a < height; a++)
    {
        for (int b = 0; b < width; b++)
        {
            image[a][b] = imageBlur[a][b];
        }
    }

    return;
}
