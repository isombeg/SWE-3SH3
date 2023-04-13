#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define CYLINDER_MAX 299
#define MAX_REQUESTS 20 // maximum number of requests
int requests[MAX_REQUESTS];
int sorted_requests[MAX_REQUESTS];


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
    c_scan(f_out, initial_position, direction, sorted_requests);

    fclose(f_out);

    return 0;
}