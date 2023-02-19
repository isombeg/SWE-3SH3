#include "context.h"

#include <stdio.h>
#include <unistd.h>

void program(Student* student){
    printf("student %d: programming\n", student->studentId);
    sleep(randnum(2, 5));
}

void program_next(Student* student){
    printf("student %d: trying to enter queue\n", student->studentId);
    try_entering_queue(student);
}

void wake_ta(Student* student){
    printf("student %d: waking TA\n", student->studentId);
    waking_ta(student);
}

void wake_ta_next(Student* student){
    get_serviced(student);
    printf("student %d: now getting serviced\n", student->studentId);
}

void wait_in_queue(Student* student){
    printf("student %d: waiting in queue\n", student->studentId);
    return;
}

void wait_in_queue_next(Student* student){
    wait_until_called(student);
    printf("student %d: called by ta\n", student->studentId);
}

void receive_help(Student* student){
    printf("student %d: receiving help\n", student->studentId);
    // wait for random period of time
    sleep(randnum(1, 5));
}

void receive_help_next(Student* student){
    dismiss_self(student);
    printf("student %d: dismissed ta\n", student->studentId);
}