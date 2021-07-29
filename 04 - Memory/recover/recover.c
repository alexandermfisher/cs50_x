#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BLOCK_SIZE 512    // define block size, i.e treat file as blocks of 512 BYTES.
typedef uint8_t BYTE;

// declare functions to be used.
int read_block(void *tmp, FILE *stream);
void check_if_start_of_jpeg(BYTE *block, FILE **outptr);
void write_block(BYTE *block, FILE *stream);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 2) {
        fprintf(stderr, "Usage: ./recover infile\n");
        return 1;
    }

     // get file name of forensic image:
    char *infile = argv[1];

    // Open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL) {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 1;
    }

    // allocate memory for block of 512 bytes to be read in.
    BYTE (*block) = malloc(BLOCK_SIZE * sizeof(BYTE));
    if (block == NULL) {
        fprintf(stderr, "Not enough memory to store image data.\n");
        fclose(inptr);
        return 1;
    }

    // initialise number of pics recovered, utilised for naming output files, bytes read and file pointer
    int bytes_read;
    FILE *outptr = NULL;

    // while there are still remaining bytes to read continue recovery of jpegs.
    do {
        // read in data from inpt file into block, return bytes read and break loop if 0.
        bytes_read = read_block(block, inptr);
        if (bytes_read == 0) {
            break;
        }

        // check if block is start of jpeg, if yes close current jpeg and open next
        check_if_start_of_jpeg(block, &outptr);

        // write data to open file
        write_block(block, outptr);

    } while (bytes_read != 0);

    fclose(inptr);
    fclose(outptr);
    free(block);

    return 0;
}

// read in next block of data (starting at given pointer ptr) from file, and return bytes read by fread()
int read_block(void *tmp, FILE *stream)
{   
    // read in 512 bytes i.e one block from file stream into tmp variable.
    int bytes_read = fread(tmp, sizeof(BYTE), BLOCK_SIZE, stream);

    return bytes_read;
}

// check to see if block is begininng of a jpeg.
void check_if_start_of_jpeg(BYTE *block, FILE **outptr)
{
    // keep track of number of pics recovered for naming files.
    static int jpegs_recovered = 0;

    // check first four bits are jpgeg signiture.
    if (block[0] == 0xFF && block[1] == 0xd8 && block[2] == 0xFF) {
        if (block[3] >> 4 == 0xe) {
            // close current file if one exists
            if (*outptr != NULL) {
                int f = fclose(*outptr);
                if (f == EOF) {
                    char console_message[36];
                    fprintf(stderr, "Failed to close file:    ./%i.jpg\n", jpegs_recovered);
                    return;
                }
            }

            // Open output file
            char outfile_name[36];
            sprintf(outfile_name, "./%.3i.jpg", jpegs_recovered);
            *outptr = fopen(outfile_name, "w");
            if (*outptr == NULL) {
                fclose(*outptr);
                fprintf(stderr, "Could not create %s.\n", outfile_name);
                return;
            }

            jpegs_recovered++;

            return;
        }
    }

    return;
}

// write block of data to output file.
void write_block(BYTE *block, FILE *stream)
{
    // in case start of image has not been found and stream is still NULL return.
    if (stream == NULL){
        return;
    }

    fwrite(block, sizeof(BYTE), BLOCK_SIZE, stream);
    
    return;
}

