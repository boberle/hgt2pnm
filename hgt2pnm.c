

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <math.h>
#include <string.h>

#define EXAGERATION 1 // try 0.4, 0.6, etc.

void read_file(FILE *, FILE *, int, int, int, int *, int *);

int main(int argc, char **argv) {

   // check whether an int is 4 bytes in this implementation
   if (sizeof(int) != 4) {
      printf("*** int is not 4 bytes!!! ***\n");
      return 1;
   }

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

   /* computing image size:
    * because data are on 16 bits, the number of pixel is size/2,
    * and because the image is a square, one side is sqrt(size/2).
    */
   struct stat st;
   stat(infile, &st);
   int filesize = st.st_size;
   int side = (int)sqrt(filesize /2 );

   printf("side length: %d\n", side);

   int max, min;
   read_file(in_fp, NULL, 0, side, 0, &max, &min);

   max = pow(max, EXAGERATION);

   int offset = (int)(max-min)/2;

   printf("max: %d\n", max);
   printf("min: %d\n", min);
   printf("offset: %d\n", offset);

   max += offset;

   fprintf(out_fp, "P2\n%d %d\n%d\n", side, side, max);

   fseek(in_fp, 0, SEEK_SET);
   int invert = max;
   read_file(in_fp, out_fp, invert, side, offset, &max, &min);

   fclose(in_fp);
   fclose(out_fp);

}


void read_file(FILE *in_fp, FILE *out_fp, int invert, int side, int offset,
      int *max, int *min) {

   int row, col;
   *max = 0;
   *min = 32767;

   char bytes[4];
   int *result = (int *)bytes;

   for (row=0; row<side; row++) {

      for (col=0; col<side; col++) {

         *result = 0;
         fread(&bytes[1], 1, 1, in_fp);
         fread(&bytes[0], 1, 1, in_fp);

         // absurd values
         if (*result > 10000 || *result < -10000) {
            *result = 0;
         }

         // exageration, only if outputing
         if (out_fp) {
            *result = pow(*result, EXAGERATION);
         }


         if (offset && *result) {
            *result += offset;
         }


         if (invert) {
            *result = invert - *result;
         }


         if (out_fp) {
            //fwrite(result, sizeof(int), 1, out_fp);
            fprintf(out_fp, "%d ", *result);
         }

         if (*result > *max) *max = *result;
         if (*result < *min) *min = *result;

      }

   }


}
