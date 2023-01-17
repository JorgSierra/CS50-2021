// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;    // For headers bytes
typedef int16_t SAMPLE;  // For file samples

// Number of bytes in .wav header
const int HEADER_SIZE = 44;   //First 44-bytes of the file are the header


int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file
    BYTE byte;
    for (int i = 1; i <= HEADER_SIZE; i++)
    {
        fread(&byte, sizeof(BYTE), 1, input);
        fwrite(&byte, sizeof(BYTE), 1, output);
    }
    // TODO: Read samples from input file and write updated data to output file
    SAMPLE sample;
    while (fread(&sample, sizeof(SAMPLE), 1, input))
    {
        sample *= factor;
        fwrite(&sample, sizeof(SAMPLE), 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);
}
