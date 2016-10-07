/*

quick file dumper for the ii2d project

Takes the base.vms file used in the II2D project and stores its contects in a header file that can be used instead.

25/01/2001 - cman@jumpstation.co.uk

No copyright is implied or imposed

*/

#include <stdio.h>


int main(int argc, char *argv[]){

    FILE *f_base,*f_src,*f_dest;

    unsigned char headerNonDynamic[0x28C];	// remembering everything is 0 based

    int c;

    fprintf(stderr,"asciidump\n"); 
    if((f_base=fopen("base.vms", "rb"))==NULL) {
        fprintf(stderr, "Failed to open base file for input.\n");
  	  return 1;
    }

    if((f_dest=fopen("ii2d_HEADER.h", "w"))==NULL) {
        fprintf(stderr, "Failed to open destinaton file for output.\n");
        fclose(f_base);
  	  return 1;
    }

// read in the header up to the dynamic bit
    if(fread(headerNonDynamic, sizeof(headerNonDynamic), 1, f_base) != 1) {
        fprintf(stderr, "Failed to read base VMS header.\n");
        fclose(f_base);
        fclose(f_dest);
        return 1;
    }

    fprintf(f_dest,"unsigned char headerNonDynamic[0x28C] = { %d",headerNonDynamic[0]);	
    for (c=1;c<sizeof(headerNonDynamic);c++) {
	fprintf(f_dest,",%d",headerNonDynamic[c]);	
    }
    fprintf(f_dest,"};");	


// as we have finished with the base, close it.
    fclose(f_base);

    fclose(f_dest);
    fprintf(stderr, "asciidump.\n");
    return 0;
}

