/**
 * Copies a BMP piece by piece, just because.
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // remember resize factor of n
    int n = atoi(argv[1]);

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    //create copy of old BITMAPINFOHEADER so we can refer to old properties when required
    BITMAPINFOHEADER oldbi = bi;

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    //alter some of required fields in BITMAPINFOHEADER.  Need to this first to be able to calculate padding.
    bi.biWidth *= n;
    bi.biHeight *= n;

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int oldpadding = (4 - (oldbi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    //then go back and calculate size of image in bytes - using the padding as calculated
    bi.biSizeImage = ((bi.biWidth * sizeof(RGBTRIPLE)) + padding) * abs(bi.biHeight);

    //alter required fields in BITMAPFILEHEADER - using biSizeImage as calculated above
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, OldbiHeight = abs(oldbi.biHeight); i < OldbiHeight; i++)
    {

        //write each scanline n times
        for (int l = 0; l < n; l++)
        {
            //store the pointer location at the start of the line - we will have to bring it back here to do the scanline multiple times
            long pos = ftell(inptr);

            // iterate over pixels in scanline
            for (int j = 0; j < oldbi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                //write the pixel k times (thus multiplying it by a factor of k)
                for (int k = 0; k < n; k++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }

            }

            // skip over padding, if any (have to refer to the original padding size)
            fseek(inptr, oldpadding, SEEK_CUR);

            // then add it back (to demonstrate how) (this refers to new padding size)
            for (int k = 0; k < padding; k++)
            {
                fputc(0x00, outptr);
            }

            //reset the pointer to the start of the scanline for the next iteration, unless we're exiting the loop (in which case we should move on)
            if (l != n-1)
            {
                fseek(inptr, pos, SEEK_SET);
            }

        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
