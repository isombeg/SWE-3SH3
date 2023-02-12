#include <stdio.h>
#include <unistd.h>

#define MAX_LINE 80

int main(void) {
    char *args[MAX_LINE/2 + 1]; 
    int should_run = 1;
    char command[128];

    while(should_run) {
        printf("osh > ");
        scanf("%s", command);
        fflush(stdout);

        /**
        * After reading user input, the steps are: 
        * 1. fork a child process using fork()
        * 2. child process will invoke int execvp (const char *file, char *const argv[]);
        * 3. parent will invole wait() unless command included &
        */
    }
}

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