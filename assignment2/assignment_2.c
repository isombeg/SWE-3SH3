#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h> 
#include <string.h> 
#include <fcntl.h>
#include <stdlib.h>

#define BUFFER_SIZE 10

#define OFFSET_MASK 0xFF //Lower 8 bits represent offset
#define OFFSET_BITS 8 //2^8 = 256. Since page size = 256
#define PAGE_SIZE 256 //Given in assignment doc
#define PAGE_COUNT 256
#define  TLB_SIZE 16

#define MEMORY_SIZE 32768

signed char *mmapfptr; 

int page_table[PAGE_COUNT] = {-1};
int pm_ptr = 0; //pointer to physical memory
char physical_memory[MEMORY_SIZE] = {-1}; //init all to -1

// Struct representing TLB entry
typedef struct {
    int page_num, frame_num;
} TLBEntry_t ;

// TLB data structure
typedef struct {
    TLBEntry_t frame[TLB_SIZE];
    int next_entry;
} Tlb;

Tlb tlb = {{-1, -1}, 0};

/**************** TLB Functionality ****************/

//searching for TLB hit
int search_tlb(Tlb* tlb, int page_num){
    for(int i = 0; i < TLB_SIZE; i++){
        if(tlb->frame[i].page_num == page_num){
            return tlb->frame[i].frame_num;
        }
    }
    return -1;
}

// Add association to TLB
void add_tlb(Tlb* tlb, int page_num, int frame_num){
    tlb->frame[tlb->next_entry++] = (TLBEntry_t){page_num, frame_num};
    tlb->next_entry %= TLB_SIZE;
}

// Update an entry in TLB
void update_tlb(Tlb* tlb, int page_num, int frame_num){
    for(int i = 0; i < TLB_SIZE; i++){
        if(tlb->frame[i].frame_num == frame_num){
            tlb->frame[i].page_num = page_num;
        }
    }
}


//finding next available frame in memory
int get_available_frame(int page_num) {

    //nothing needs to be replaced. simply add entries to page and tlb tables
    if(physical_memory[pm_ptr * PAGE_SIZE] == -1) {
        page_table[page_num] = pm_ptr;
        add_tlb(&tlb, page_num, pm_ptr);
    } 

    //update 2 entries of page table: corresponding to page being replaced, and other new page
    else {
        for(int i = 0; i < PAGE_COUNT; i++) {
            if(page_table[i] == pm_ptr) {
                page_table[i] = -1;
                break;
            }
        }
        page_table[page_num] = pm_ptr;
        update_tlb(&tlb, page_num, pm_ptr);
    }

    //logic for FIFO wrapping of pm_ptr (physical_memory pointer)
    int original_ptr_val = pm_ptr;
    pm_ptr = (pm_ptr + 1) % (PAGE_SIZE/2);
    return original_ptr_val;
}

//copy page in from memory mapped BACKING_STORE.bin and place it into an available frame in physical memory
int handle_page_fault(int page_num) {
    
    //get next available frame, uses FIFO ordering scheme
    int available_frame = get_available_frame(page_num);
    memcpy(&physical_memory[available_frame * PAGE_SIZE], mmapfptr + PAGE_SIZE * page_num, PAGE_SIZE); 
    return available_frame;
}

void save_to_memory() {
    //open file in read only mode
    int mmapfile_fd = open("BACKING_STORE.bin", O_RDONLY);

    int mem_size = 65536;

    //map to memory the backing store
    mmapfptr = mmap(0, mem_size, PROT_READ, MAP_PRIVATE, mmapfile_fd, 0);
}

int main() {
    
    //metrics to track
    int page_faults = 0;
    int tlb_hits = 0;
    int addresses = 0;

    //reading backing.bin from memory and saving to memory mapped location
    save_to_memory();

    // Initialize entries page table
    for(int i = 0; i < PAGE_COUNT; i++)
        page_table[i] = -1;

    // Initialize entries in physical memory structure
    for(int i = 0; i < MEMORY_SIZE; i++)
        physical_memory[i] = -1;

    // Initialize TLB
    for(int i = 0; i < TLB_SIZE; i++)
        tlb.frame[i] = (TLBEntry_t){-1, -1};

    //open file for reading
    FILE * fptr;
    fptr = fopen ("addresses.txt", "r");

    //output file for saving results
    FILE * f_out;
    f_out = fopen("my_out.txt", "w+");

    char buff[BUFFER_SIZE];
    while(fgets(buff, BUFFER_SIZE, fptr) != NULL) {
        addresses++;
        
        //extract page_num, offset and computer physical addr using bitwise operators
        int logical_addr = atoi(buff);
        int page_num = logical_addr >> OFFSET_BITS;
        int page_offset = logical_addr & OFFSET_MASK;

        int frame_num = 0;
        
        //successful tlb
        if(search_tlb(&tlb, page_num) != -1) {
            tlb_hits++;
            frame_num = search_tlb(&tlb, page_num);
        }

        //search in page table for corresponding frame num
        else if(page_table[page_num] != -1) {
            frame_num = page_table[page_num];
        }

        //page does not exist in physical memory yet
        else {
            page_faults++;
            frame_num = handle_page_fault(page_num);
        }

        //computing physical address based on bitwise operations
        int physical_addr = (frame_num << OFFSET_BITS) | page_offset;
        
        //writing to file
        fprintf(f_out, "Virtual address: %d Physical address = %d Value=%d \n", logical_addr, physical_addr, physical_memory[physical_addr]);
    } 
    fprintf(f_out, "Total addresses = %d \nPage_Faults = %d \nTLB Hits = %d \n", addresses, page_faults, tlb_hits);
    fclose(f_out);
    fclose(fptr);

    return 0;
}
