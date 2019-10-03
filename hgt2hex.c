/*
 * Convert `hgt` files to a human readable hexadecimal representation.
 * 
 * `hgt` files are files containing elevation files from NASA.  Learn more
 * here: https://dds.cr.usgs.gov/srtm/version2_1/Documentation/
 *
 * Each file is a tile.  The tile size is defined by the constant
 * `TILE_SIZE`.  The output is as follows:
 *
 * 0166 016B 0172 017C 017F 0183 0184 0180 0181 0187 018D
 * 0191 0197 0198 01A1 01A9 01AF 01B6 01BB 01BB 01BA 01BB
 * 01BE 01C6 01CE 01D1 01D2 01CD 01C8 01C2 01C0 01C0 01BD
 * ...
 *
 * There are `TILE_SIZE` columns, and `TILE_SIZE` rows.
 *
 *
 * Author: Bruno Oberle (boberle.com)
 *
 */

#include <stdlib.h>
#include <stdio.h>

#define TILE_SIZE 1201

int main(int argc, char **argv)
{

   if (argc != 2)
   {
      printf("Usage: hgt2hex filename > out\n");
      return 1;
   }

   char *filename = *(argv+1);
   //printf("Treating file '%s'...\n", filename);

   FILE *fp = fopen(filename, "r");
   if (fp == NULL)
   {
      printf("*** can't open the file ***\n");
      return 1;
   }

   if (sizeof(int) != 4)
   {
      printf("*** int is not 4 bytes!!! ***\n");
      return 1;
   }

   char bytes[4];
   int *result = (int *)bytes;

   int row, col;
   for (row=0; row<TILE_SIZE; row++)
   {

      for (col=0; col<TILE_SIZE; col++)
      {

         *result = 0;
         fread(&bytes[1], 1, 1, fp);
         fread(&bytes[0], 1, 1, fp);
         printf("%04X ", *result);

      }

      printf("\n");

   }

   // TODO: has everything been read?

   fclose(fp);

}
