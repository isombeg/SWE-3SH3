#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 10

#define OFFSET_MASK 0xFF //Lower 8 bits represent offset
#define OFFSET_BITS 8 //2^8 = 256. Since page size = 256
#define PAGE_SIZE 256 //Given in assignment doc

char buff[BUFFER_SIZE];
int page_table[8] = {6,4,3,7,0,1,2,5};

int address_translation() {
    //open file for reading
    FILE * fptr;
    fptr = fopen ("addresses.txt", "r");
    while(fgets(buff, BUFFER_SIZE, fptr) != NULL) {

        //extract page_num, offset and computer physical addr using bitwise operators
        int logical_addr = atoi(buff);
        int page_num = logical_addr >> OFFSET_BITS;
        int page_offset = logical_addr & OFFSET_MASK;

        int frame_num = 0;
        //successful tlb
        if(tlb_lookup) {
            tlb_lookup(&frame_num);
        }
        else {
            frame_num = page_table[page_num];
        }
        int physical_addr = (frame_num << OFFSET_BITS) | page_offset;

    } 
    
    fclose(fptr);
}

int tlb_lookup(int *frame_num) {
    //implement TLB STUFF
    return 0;
}
int main () {
   address_translation();
   return(0);
}