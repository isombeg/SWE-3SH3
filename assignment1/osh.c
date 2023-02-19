#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 40
#define HISTORY_MAX_SIZE 5

char* command_history[HISTORY_MAX_SIZE];
int history_queue_count = 0, history_front = 0, commands_count = 0;

void exec_command(char* args_str, char **args, int *should_run, int should_record_in_history);

void free_and_allocate(char ** args) {
    for(int i = 0; i < MAX_LINE; i++)
        free(args[i]);

    for(int i = 0; i < MAX_LINE; i++) {
        args[i] = (char*)malloc(128 * sizeof(char));
        memset(args[i], 0, sizeof(args[i]));
    }
}

int split_string(const char* str, char** args) {
    int i = 0;
    free_and_allocate(args);
    char* str_copy = malloc(strlen(str) * sizeof(char*)); 
    strcpy(str_copy, str);
    char * token = strtok(str_copy, " ");
    // loop through the string to extract all other tokens
    while( token != NULL ) {
        strcpy(args[i++], token); //printing each token
        token = strtok(NULL, " ");
    }

    free(str_copy);

    if(i == 0) {
        args[i + 1] = NULL;
        return 0;
    }
    args[i] = NULL; //execvp requires last member of this array is a NULL pointer.
    return i - 1; // last index of args
}

int is_history_full(){
    return history_queue_count == HISTORY_MAX_SIZE;
}

void record_history(char* command){
    
    if(!is_history_full())
        strcpy(command_history[history_queue_count++], command );
    else {
        strcpy(command_history[history_front], command);
        history_front = (history_front + 1) % HISTORY_MAX_SIZE;
    }
    commands_count++;
}

void display_history(){
    for(int i = 0; i < history_queue_count; i++)
    {
        if(is_history_full())
            fprintf(stdout, "%d %s\n", commands_count - i, command_history[((history_front - i - 1) + HISTORY_MAX_SIZE) % HISTORY_MAX_SIZE]);
        else
            fprintf(stdout, "%d %s\n", commands_count - i, command_history[history_queue_count - i - 1]);
    }
    fflush(stdout);
}

void peek_history(char* args_str, int should_run_concurrent) {
    if(history_queue_count == 0)
        strcpy(args_str, "echo No commands in history.");
    else {
        if(!is_history_full()) {
            strcpy(args_str, command_history[history_queue_count - 1]);
        }
        else {
            strcpy(args_str, command_history[(history_front + HISTORY_MAX_SIZE - 1) % HISTORY_MAX_SIZE]);
        }
        if(should_run_concurrent)
            strcat(args_str, " &");
    }
}

int main(void) {

    char** args = malloc(MAX_LINE * sizeof(char *));

    char *args_str = malloc(128*sizeof(char));
    int should_run = 1;

    for(int i = 0; i < HISTORY_MAX_SIZE; i++) {
        command_history[i] = (char*) malloc(128 * sizeof(char)); /*+1 for '\0' character */
    }

    while(should_run) {
        memset(args_str, 0, sizeof(args_str)); //clears arg str

        fprintf(stdout, "osh > ");
        fflush(stdout);

        fgets(args_str, 128, stdin);

        exec_command(args_str, args, &should_run, 1);

        fflush(stdout);
    }

    return 0;
}


void exec_command(char* args_str, char **args, int *should_run, int should_record_in_history){

    int should_run_concurrent;
    int status;
    pid_t pid;
    args_str[strcspn(args_str, "\n")] = 0; //removes new line character
    
    // Split string into array of strings
    int args_last_i = split_string(args_str, args);

    if(strlen(args_str) == 0) 
        should_record_in_history = 0;

    // Indicate if command should run concurrent
    should_run_concurrent = 0;
    if(!strcmp(args[args_last_i], "&")) {
        should_run_concurrent = 1;
        // strip ampersand from string
        args[args_last_i] = '\0';
    }

    // Hook for executing last command
    if(!strcmp(args[0], "!!")){
        peek_history(args_str, should_run_concurrent);
        // should_record_in_history = 0;
        exec_command(args_str, args, should_run, 0);
        return;
    }

    if(!strcmp(args[0], "exit")) {
        *should_run = 0;
        return;
    }
    if(!strcmp(args[0], "history")){
        display_history();
        if(should_record_in_history)
            record_history(args_str);
        return;
    }

    pid = fork();

    //child process
    if(pid == 0) {
        execvp(args[0], args);
        fprintf(stdout, "Error\n");
        exit(0);
    } 
    //parent process
    else if (pid > 0) {

        if(should_run_concurrent)
            waitpid(pid, &status, WNOHANG);
        else
            waitpid(pid, &status, 0);
    }
    //error
    else {
        fprintf(stdout, "An error occured");
        *should_run = 0;
    }

    if(should_record_in_history)
        record_history(args_str); 
}