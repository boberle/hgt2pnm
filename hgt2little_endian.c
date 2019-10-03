/*
 * Convert `hgt` files from big endian to little endian.
 * 
 * `hgt` files, which contain elevation files from NASA (learn more
 * here: https://dds.cr.usgs.gov/srtm/version2_1/Documentation/), are
 * big endian.  This program just convert them to little endian.
 *
 *
 * Author: Bruno Oberle (boberle.com)
 *
 */

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {

   if (argc != 3) {
      printf("Usage: hgt2hex infile outfile\n");
      return 1;
   }

   char *infile = *(argv+1);
   char *outfile = *(argv+2);

   FILE *in_fp = fopen(infile, "rb");
   if (in_fp == NULL) {
      printf("*** can't open the file '%s' ***\n", infile);
      return 1;
   }

   FILE *out_fp = fopen(outfile, "wb");
   if (out_fp == NULL) {
      printf("*** can't open the file '%s' ***\n", outfile);
      return 1;
   }

   char one;
   char two;

   while (fread(&one, 1, 1, in_fp)) {
      fread(&two, 1, 1, in_fp);
      fwrite(&two, sizeof(char), 1, out_fp);
      fwrite(&one, sizeof(char), 1, out_fp);
   }

}
