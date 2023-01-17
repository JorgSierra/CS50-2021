#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#define blockSize 512

typedef uint8_t BYTE;

int isHeader(BYTE currentBlock[blockSize]);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: recover file\n");
        return 1;
    }
    // Remember filenames
    char *infile = argv[1];

    // Open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s\n", infile);
        return 1;
    }
    // Stack a buffer
    BYTE buffer[blockSize];
    // Pointer to output file
    FILE *outptr = NULL;
    // Variables for file naming
    char fileName[8] = "xxx.jpg";
    int counter = 0;
    
    // While there is data to read on the file read and process
    while (fread(buffer, sizeof(BYTE), blockSize, inptr) && feof(inptr) == 0)
    {
        // Prepare the actual count file name
        sprintf(fileName, "%03i.jpg", counter);
        // If is not a header
        if (!isHeader(buffer))
        {   
            // If it has found the fist header
            if (outptr != NULL)
            {
                fwrite(buffer, sizeof(BYTE), blockSize, outptr);
            }
        }
        else
        {   
            // If there is already a file in use
            if (outptr != NULL)
            {
                // Close the later file
                fclose(outptr);
                outptr = NULL;
            }
            // Open a new file
            outptr = fopen(fileName, "w");
            fwrite(buffer, sizeof(BYTE), blockSize, outptr);
            counter++;
        }
    }
}

int isHeader(BYTE currentBlock[blockSize])
{
    // if the current block contains a JPG header 
    if (currentBlock[0] == 0xff &&
        currentBlock[1] == 0xd8 &&
        currentBlock[2] == 0xff &&
        (currentBlock[3] & 0xf0) == 0xe0)
    {
        return 1;
    }
    return 0;
}