#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int isjpeg(uint8_t block[]);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover file\n");
        return 1;
    }

    //open infile
    FILE *infile = fopen(argv[1], "r");
    if (infile == NULL)
    {
        fprintf(stderr, "Error: could not open infile.\n");
        return 2;
    }

    //declare outfile pointer but initialise it to a dummy value for now
    FILE *outfile = NULL;

    //temporary buffer for storage of a 512 byte block at a time
    uint8_t block[512];

    //set up filename counter
    char filename[strlen("000.jpg") + 1]; //add 1 for NULL character!!
    int filecount = 0;

    while (fread(&block, sizeof(block), 1, infile) == 1)
    {
        //check whether start of jpeg file
        if (isjpeg(block) == 1)
        {
            //check whether we already have a file open
            if (outfile != NULL)
            {
                fclose(outfile);    //close the outfile
                filecount++;        //increment filenumber
            }

            //open outfile
            sprintf(filename, "%03i.jpg", filecount);

            outfile = fopen(filename, "w");
            if (outfile == NULL)
            {
                fprintf(stderr, "Error: could not open outfile.\n");
                return 3;
            }
        }

        //make sure we have a file open before trying to write - extra sanity check as there may be dummy data at the start of the raw file
        if (outfile != NULL)
        {
            //write the block to outfile
            fwrite(&block, sizeof(block), 1, outfile);
        }
    }

    fclose(infile);
    fclose(outfile);
    return 0;
}

int isjpeg(uint8_t block[])
{
    //checks whether the block indicates the start of a jpeg
    if (block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && block[3] >= 0xe0 && block[3] <= 0xef)
    {
        //true
        return 1;
    }
    else
    {
        //false
        return 0;
    }
}