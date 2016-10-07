/*

II2D - Images In2 Dreams Version 1.2

Coded by rednuht - ii2d@jumpstation.co.uk - www.jumpstation.co.uk

supplied without any warenty, if it works, good, if it phux your data, then thats 2 bad, don't blame me.

version history
20/01/01 1.0 using hard coded values, the test jpg image is correctly converted and worked in JSR.
25/01/01 1.2 using fseek and ftell getting the file size is very easy
 - hacked a quick ascii dump program to generate the header file to use instead of loading from a base file.
 - added command line arguments and help
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ii2d_HEADER.h"

void help();

int main(int argc, char *argv[]){

    FILE *f_src,*f_dest;

    unsigned char mimeHeader[0x30];	// 48

    unsigned char *imgData;

    int fileSize,sizeA,sizeC,c;

    float sizeB,sizeD;

    if (argc!=3) {
	help();
	return 1;
    }

    if (strlen(argv[1])>35) {
	fprintf(stderr,"Input file name to long, rename file and try again.\n");
	return 1;
    }

    fprintf(stderr,"II2D c_man@jumpstation.co.uk\n");
//    fprintf(stderr,"II2D open source file for reading\n");
    if((f_src=fopen(argv[1], "rb"))==NULL) {
          fprintf(stderr, "Failed to open source file [%s] for input.\n",argv[1]);
          return 1;
    }
//    fprintf(stderr,"II2D open destination file for writing\n");
    if((f_dest=fopen(argv[2], "wb"))==NULL) {
          fprintf(stderr, "Failed to open destinaton file [%s] for output.\n",argv[2]);
          fclose(f_src);
  	  return 1;
    }

//  save header in the destination file.
    if(fwrite(headerNonDynamic, sizeof(headerNonDynamic), 1, f_dest) != 1) {
        fprintf(stderr, "Failed to write base header to destination file [%s].\n",argv[2]);
        fclose(f_src);
        fclose(f_dest);
        return 1;
    }

// read source file size

    fseek(f_src,0,SEEK_END);	// switch to the end, offset by 0 bytes
    fileSize = ftell(f_src);	// read number of bytes at the end of file, i.e. file size.

    fseek(f_src,0,SEEK_SET);	// switch the begining of the file for future access

//    fprintf(stderr,"It is %d bytes long\n",fileSize);

    if((imgData = (char *)malloc(fileSize))==NULL) {
        fprintf(stderr, "The image file data is to large for current memeory configuration, re-configure memory or ue a smaller image or better compression\n");
        fclose(f_src);
        fclose(f_dest);
        return 1;
    }

    if(fread(imgData, fileSize, 1, f_src) != 1) {
        fprintf(stderr, "Failed to read image file data.\n");
        fclose(f_src);
        fclose(f_dest);
        return 1;
    }

// as we have finished with the source file, close it
    fclose(f_src);

// calculate size bytes
    sizeD = (float)fileSize / 256;
    sizeA = (int)sizeD;
    sizeB = sizeD - sizeA;
    sizeC = (int)(sizeB * 256);

//    fprintf(stderr, "sizeA = %d\n",sizeA);
//    fprintf(stderr, "sizeB = %f\n",sizeB);
//    fprintf(stderr, "sizeC = %d\n",sizeC);
//    fprintf(stderr, "sizeD = %f\n",sizeD);

// write size bytes

    putc(sizeC,f_dest);
    putc(sizeA,f_dest);

// pad nulls

    putc(0,f_dest);
    putc(0,f_dest);


// write mime header

    mimeHeader[0]  = 'i';
    mimeHeader[1]  = 'm';
    mimeHeader[2]  = 'a';
    mimeHeader[3]  = 'g';
    mimeHeader[4]  = 'e';
    mimeHeader[5]  = '/';
// image type, but as this is 4 JSR I just letting it use JPG and not gif, although it should be easy to change
    mimeHeader[6]  = 'j';
    mimeHeader[7]  = 'p';
    mimeHeader[8]  = 'e';
    mimeHeader[9]  = 'g';
    mimeHeader[10] = ' ';
    mimeHeader[11] = ';';
    mimeHeader[12] = ' ';

//image filename
// check length and complain
    strcat(mimeHeader,argv[1]);
//    fprintf(stderr,"[%s]\n",mimeHeader);


// pad with nulls
    for (c=21;c<48;c++) {
        mimeHeader[c] = 0;
    }

    if(fwrite(mimeHeader, sizeof(mimeHeader), 1, f_dest) != 1) {
        fprintf(stderr, "Failed to write mime header to destination file.\n");
        fclose(f_dest);
        return 1;
    }

// write source image

    if(fwrite(imgData, fileSize, 1, f_dest) != 1) {
        fprintf(stderr, "Failed to write image data to destination file.\n");
        fclose(f_dest);
        return 1;
    }

    free(imgData);
    fclose(f_dest);
    fprintf(stderr, "II2D is not aware of any problems with your file transformation.\n");
    fprintf(stderr, "[%s] is now ready.\n", argv[2]);
    return 0;

}

void help() {
    fprintf(stderr,"II2D <image> <vms>\n");
    fprintf(stderr,"<image> should be a jpeg\n");
    fprintf(stderr,"<vms> is the file name to produce\n");
    fprintf(stderr,"II2D c_man@jumpstation.co.uk\n");

}
