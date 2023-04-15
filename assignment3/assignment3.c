#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define CYLINDER_MAX 299
#define MAX_REQUESTS 20 // maximum number of requests
int requests[MAX_REQUESTS];
int sorted_requests[MAX_REQUESTS];

int scan_one_dir(FILE* fptr, int initial_head, int start_pos_idx, int total_head_movements, const char* direction, int requests[]);
int scan_opposite_dir(FILE* fptr, int start_pos_idx, int total_head_movements, const char* direction, int requests[], int start_from_extreme);
int scan_from_extreme_to_start(FILE* fptr, int initial_head, int start_pos_idx, int total_head_movements, const char* direction, int requests[]);
int scan_left(FILE* fptr, int initial_head, int start_pos_idx, int end_pos_idx, int total_head_movements, int requests[]);
int scan_right(FILE* fptr, int initial_head, int start_pos_idx, int end_pos_idx, int total_head_movements, int requests[]);
int find_start_pos(int initial_head, const char* direction, int requests[]);
int record_movement_to_extreme(int total_head_movements, const char* direction, int requests[]);

void fcfs(FILE* fptr, int initial_head, int requests[]) {
    int curr_head = initial_head;
    int total_head_movements = 0;
    fprintf(fptr, "FCFS DISK SCHEDULING ALGORITHM: \n\n");
    
    for(int i = 0; i < MAX_REQUESTS; i++) {
        int next_pos = requests[i];
        total_head_movements += abs(curr_head - next_pos);
        curr_head = next_pos;
        fprintf(fptr, "%d, ", next_pos);
    }

    fprintf(fptr, "\n\nFCFS - Total head movements = %d \n\n", total_head_movements);
}

void sstf(FILE* fptr, int initial_head, int requests[]) {
    int curr_head = initial_head;
    int total_head_movements = 0;
    int requests_served = 0;
    fprintf(fptr, "SSTF DISK SCHEDULING ALGORITHM: \n\n");

    //create a copy of initial array
    int copied_arr[MAX_REQUESTS];
    for(int i = 0; i < MAX_REQUESTS; i++)
        copied_arr[i] = requests[i];

    //first find closest request to curr head pos
    while(requests_served < MAX_REQUESTS) {
        int min_diff = INT_MAX;
        int next_pos_idx = 0;
        for(int i = 0; i < MAX_REQUESTS; i++) {
            if(abs(curr_head - copied_arr[i]) < min_diff) {
                min_diff = abs(curr_head - copied_arr[i]);
                next_pos_idx = i;
            }
        }
        total_head_movements += abs(curr_head - copied_arr[next_pos_idx]);
        curr_head = copied_arr[next_pos_idx];
        fprintf(fptr, "%d, ", copied_arr[next_pos_idx]);
        copied_arr[next_pos_idx] = INT_MAX;
        requests_served++;
    }
    fprintf(fptr, "\n\nSSTF - Total head movements = %d \n\n", total_head_movements);   
}

void scan(FILE* fptr, int initial_head, const char* direction, int requests[]){
    fprintf(fptr, "SCAN DISK SCHEDULING ALGORITHM: \n\n");
    // Find starting point
    int start_pos_idx = find_start_pos(initial_head, direction, requests);

    int total_head_movements = 0;
    // Scan all the way to extremity
    total_head_movements = scan_one_dir(fptr, initial_head, start_pos_idx, total_head_movements, direction, requests);
    // Scan to next extremity from the original extremity
    total_head_movements = scan_opposite_dir(fptr, start_pos_idx, total_head_movements, direction, requests, 1);

    fprintf(fptr, "\n\nSCAN - Total head movements = %d \n\n", total_head_movements);

}

int scan_one_dir(FILE* fptr, int initial_head, int start_pos_idx, int total_head_movements, const char* direction, int requests[]){
    if(strcmp(direction, "LEFT") == 0){
        //go all the way left from starting point
        return scan_left(fptr, initial_head, start_pos_idx, 0, total_head_movements, requests);
    } else {
        // go all the way right from starting point
        return scan_right(fptr, initial_head, start_pos_idx, MAX_REQUESTS-1, total_head_movements, requests);
    }
}

int scan_opposite_dir(FILE* fptr, int start_pos_idx, int total_head_movements, const char* direction, int requests[], int start_from_extreme){
    int initial_head;
    if(strcmp(direction, "LEFT") == 0){
        // determine whether starting point should be extreme cylinder or request address based on flag
        // and record head movements accordingly
        initial_head = start_from_extreme ? 0 : requests[0];
        total_head_movements += start_from_extreme ? abs(requests[0] - 0) : 0;
        // go all the way right from starting point
        return scan_right(fptr, initial_head, start_pos_idx+1, MAX_REQUESTS-1, total_head_movements, requests);
    } else {
        // determine whether starting point should be extreme cylinder or request address based on flag
        // and record head movements accordingly
        initial_head = start_from_extreme ? CYLINDER_MAX : requests[MAX_REQUESTS-1];
        total_head_movements += start_from_extreme ? abs(requests[MAX_REQUESTS-1] - CYLINDER_MAX) : 0;
        // go all the way left from starting point
        return scan_left(fptr, initial_head, start_pos_idx-1, 0, total_head_movements, requests);
    }
}

int scan_left(FILE* fptr, int initial_head, int start_pos_idx, int end_pos_idx, int total_head_movements, int requests[]){
    int curr_head = initial_head;
    for(int i = start_pos_idx; i >= end_pos_idx; i--) {
        total_head_movements += abs(curr_head - requests[i]);
        curr_head = requests[i];
        fprintf(fptr, "%d, ", requests[i]);
    }
    return total_head_movements;
}

int scan_right(FILE* fptr, int initial_head, int start_pos_idx, int end_pos_idx, int total_head_movements, int requests[]){
    int curr_head = initial_head;
    //go all the way right from starting point
    for(int i = start_pos_idx; i <= end_pos_idx; i++) {
        total_head_movements += abs(curr_head - requests[i]);
        curr_head = requests[i];
        fprintf(fptr, "%d, ", requests[i]);
    }
    return total_head_movements;
}

int find_start_pos(int initial_head, const char* direction, int requests[]){
    int init_pos_idx;
    if(strcmp(direction, "LEFT") == 0) {
        //first find starting point (first request that is smaller than our initial head)
        for (init_pos_idx = MAX_REQUESTS - 1; init_pos_idx >= 0; init_pos_idx--) {
            if (requests[init_pos_idx] <= initial_head) {
                break;
            }
        }
    } else {
        for(init_pos_idx = 0; init_pos_idx < MAX_REQUESTS; init_pos_idx++) {
            if(requests[init_pos_idx] >= initial_head) {
                break;
            }
        }
    }

    return init_pos_idx;
}

void c_scan(FILE* fptr, int initial_head, const char* direction, int requests[]) {
    int curr_head = initial_head;
    int total_head_movements = 0;
    int init_pos_idx = 0;
    fprintf(fptr, "C-SCAN DISK SCHEDULING ALGORITHM: \n\n");

    //serve head direction noted
    if(strcmp(direction, "LEFT") == 0) {
        
        //first find starting point (first request that is smaller than our initial head)
        for(int i = 0; i < MAX_REQUESTS; i++) {
            if(requests[i] > curr_head) {
                init_pos_idx = i;
                break;
            }
        }
        init_pos_idx--;

        //go all the way left from starting point
        for(int i = init_pos_idx; i >= 0; i--) {
            total_head_movements += abs(curr_head - requests[i]);
            curr_head = requests[i];
            fprintf(fptr, "%d, ", requests[i]);
        }

        //scroll to right-most 
        total_head_movements += abs(curr_head - 0);
        curr_head = CYLINDER_MAX;
        total_head_movements += abs(curr_head - 0);

        //keep going left from right-most point until serviced all requests
        for(int i = MAX_REQUESTS - 1; i > init_pos_idx; i--) {
            total_head_movements += abs(curr_head - requests[i]);
            curr_head = requests[i];
            fprintf(fptr, "%d, ", requests[i]);
        }
    }
    else {

        //first find starting point
        for(int i = 0; i < MAX_REQUESTS; i++) {
            if(requests[i] >= curr_head) {
                init_pos_idx = i;
                break;
            }
        }
        
        //go all the way right from starting point
        for(int i = init_pos_idx; i < MAX_REQUESTS; i++) {
            total_head_movements += abs(curr_head - requests[i]);
            curr_head = requests[i];
            fprintf(fptr, "%d, ", requests[i]);
        }

        //scroll to left-most 
        total_head_movements += abs(curr_head - CYLINDER_MAX);
        curr_head = 0;
        total_head_movements += abs(curr_head - CYLINDER_MAX);

        //keep going right from left-most point until serviced all requests
        for(int i = 0; i < init_pos_idx; i++) {
            total_head_movements += abs(curr_head - requests[i]);
            curr_head = requests[i];
            fprintf(fptr, "%d, ", requests[i]);
        }
    }

    fprintf(fptr, "\n\nC-SCAN - Total head movements = %d \n\n", total_head_movements);
}

void sort_requests(int requests[], int sorted_requests[], int len) {
    int i, j, temp;
    
    // Copy unsorted requests to sorted requests array
    for (i = 0; i < len; i++) {
        sorted_requests[i] = requests[i];
    }
    
    // Bubble sort algorithm
    for (i = 0; i < len-1; i++) {
        for (j = 0; j < len-i-1; j++) {
            if (sorted_requests[j] > sorted_requests[j+1]) {
                temp = sorted_requests[j];
                sorted_requests[j] = sorted_requests[j+1];
                sorted_requests[j+1] = temp;
            }
        }
    }
}

void look(FILE* fptr, int initial_head, const char* direction, int requests[]){
    fprintf(fptr, "LOOK DISK SCHEDULING ALGORITHM: \n\n");
    // Find starting point
    int start_pos_idx = find_start_pos(initial_head, direction, requests);

    int total_head_movements = 0;
    // Scan all the way to extremity
    total_head_movements = scan_one_dir(fptr, initial_head, start_pos_idx, total_head_movements, direction, requests);

    // Scan to next extremity from last cylinder
    total_head_movements = scan_opposite_dir(fptr, start_pos_idx, total_head_movements, direction, requests, 0);

    fprintf(fptr, "\n\nLOOK - Total head movements = %d \n\n", total_head_movements);

}

void c_look(FILE* fptr, int initial_head, const char* direction, int requests[]){
    fprintf(fptr, "C-LOOK DISK SCHEDULING ALGORITHM: \n\n");
    // Find starting point
    int start_pos_idx = find_start_pos(initial_head, direction, requests);

    int total_head_movements = 0;

    // Scan all the way to extremity from initial head
    total_head_movements = scan_one_dir(fptr, initial_head, start_pos_idx, total_head_movements, direction, requests);

    // Scan all the way back to last request from to request next to initial head
    total_head_movements = scan_from_extreme_to_start(fptr, initial_head, start_pos_idx, total_head_movements, direction, requests);

    fprintf(fptr, "\n\nC-LOOK - Total head movements = %d \n\n", total_head_movements);
}

int scan_from_extreme_to_start(FILE* fptr, int initial_head, int start_pos_idx, int total_head_movements, const char* direction, int requests[]){
    if(strcmp(direction, "LEFT") == 0){
        return scan_left(fptr, requests[0], MAX_REQUESTS-1, start_pos_idx+1, total_head_movements, requests);
    } else {
        return scan_right(fptr, requests[MAX_REQUESTS-1], 0, start_pos_idx-1, total_head_movements, requests);
    }
}

int main(int argc, char *argv[]) {

    // Parse initial position of the disk head
    int initial_position = atoi(argv[1]);

    // Parse direction of the head
    const char *direction = argv[2];

    //open file for reading
    FILE * fptr;
    fptr = fopen ("request.bin", "rb");

    int num_requests = 0;
    // Read requests from file and store them in integer array
    while (fread(&requests[num_requests], sizeof(int), 1, fptr) == 1) {
        num_requests++;
    }
    // Close file
    fclose(fptr);
   
    // Sort requests in increasing order
    sort_requests(requests, sorted_requests, num_requests);

    FILE * f_out;
    if(strcmp(direction, "LEFT") == 0)
        f_out = fopen("our_output_left.txt", "w+");
    else
        f_out = fopen("our_output_right.txt", "w+");

    fprintf(fptr, "Total Requests = %d \nIntial Head Position: %d \nDirection of Head: %s \n\n", MAX_REQUESTS, initial_position, direction);

    fcfs(f_out, initial_position, requests);
    sstf(f_out, initial_position, requests);
    scan(f_out, initial_position, direction, sorted_requests);
    c_scan(f_out, initial_position, direction, sorted_requests);
    look(f_out, initial_position, direction, sorted_requests);
    c_look(f_out, initial_position, direction, sorted_requests);

    fclose(f_out);

    return 0;
}