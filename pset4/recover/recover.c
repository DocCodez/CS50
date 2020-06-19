#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

int main(int argc, char *argv[])
{
    // Make sure the program accepts at most one command line argument.
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // Open the file for reading.
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        return 1;
    }

    unsigned char bytes[512];
    int amountJPEG = 0;
    char *filename = malloc(sizeof(char) * 8);
    FILE *img = NULL;
    bool foundJPEG = false;

    // While not at end of card.
    while (fread(bytes, 512, 1, file) == 1)
    {
        // Check if beginning of JPEG
        if (bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff && (bytes[3] & 0xf0) == 0xe0)
        {
            if (amountJPEG == 0)
            {
                sprintf(filename, "%03i.jpg", amountJPEG);
                img = fopen(filename, "w");
                fwrite(bytes, 512, 1, img);
                amountJPEG++;
                foundJPEG = true;
            }
            else
            {
                fclose(img);
                sprintf(filename, "%03i.jpg", amountJPEG);
                img = fopen(filename, "w");
                fwrite(bytes, 512, 1, img);
                amountJPEG++;
                foundJPEG = true;
            }
        }
        else
        {
            if (foundJPEG == true)
            {
                fwrite(bytes, 512, 1, img);
            }
        }
    }

    fclose(file);
    fclose(img);
    free(filename);

}
