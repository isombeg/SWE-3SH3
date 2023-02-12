#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 40
   
int split_string(char* str, char** args) {
    int i = 0;
    char * token = strtok(str, " ");
    // loop through the string to extract all other tokens
    while( token != NULL ) {
        args[i++] = token; //printing each token
        token = strtok(NULL, " ");
    }

    return i - 1; // last index of args
}

void clear_buffers(char* str, char** args){
    memset(str, 0, sizeof(str));

    for(int i = 0; i < MAX_LINE; i++) {
        args[i] = '\0';
    }
}

int main(void) {
    char *args[MAX_LINE]; 
    char args_str[128];
    int should_run = 1;
    int should_run_concurrent;
    int status; 
    pid_t pid; 

    while(should_run) {

        clear_buffers(args_str, args);

        fprintf(stdout, "osh > ");
        fflush(stdout);

        fgets(args_str, 128, stdin);
        args_str[strcspn(args_str, "\n")] = 0;
        
        int args_last_i = split_string(args_str, args);

        should_run_concurrent = 0;
        if(!strcmp(args[args_last_i], "&")) {
            should_run_concurrent = 1;
            // strip last string
            args[args_last_i] = '\0';
        }

        if(!strcmp(args[0], "exit")) {
            should_run = 0;
            break;
        }
        
        pid = fork();

        //child process
        if(pid == 0) {
            execvp(args[0], args);
            exit(0);
        } 
        //parent process
        else if (pid > 0) {

            if(should_run_concurrent) {
                //This flag specifies that waitpid should return immediately instead of waiting
                waitpid(-1, &status, WNOHANG);
            } else {
                wait(NULL); //parent waits for child to complete

            }
        }
        //error
        else {
            fprintf(stdout, "An error occured");
            should_run = 0;
        }
        fflush(stdout);
    }

    return 0;
}

    /**
    * After reading user input, the steps are: 
    * 1. fork a child process using fork()
    * 2. child process will invoke int execvp (const char *file, char *const argv[]);
    * 3. parent will invole wait() unless command included &
    */
    
	// pid_t pid;	
	// pid	= fork();

	// if (pid == 0) {
	// fork();
	// fork();
	// fork();
	// } 
	
	// while(wait(NULL) > 0);
	// printf("%d\n",getpid());
	// return 0;