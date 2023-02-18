#include "context.h"

#include <stdio.h>

const StudentState NO_HELP_WANTED_STATE = {
    STATE_NO_HELP_WANTED,
    program,
    program_next
};

const StudentState WAKING_TA_STATE = {
    STATE_WAKING_TA,
    wake_ta,
    wake_ta_next
};

const StudentState WAITING_IN_QUEUE_STATE = {
    STATE_WAITING_IN_QUEUE,
    wait_in_queue,
    wait_in_queue_next
};

const StudentState RECEIVING_HELP_STATE = {
    STATE_RECEIVING_HELP,
    receive_help,
    receive_help_next
};

void program(Student* student){
    printf("student %d: programming\n", student->studentId);
    sleep(randnum(1, 5));
}

void program_next(Student* student){
    try_entering_queue(student);
}

void wake_ta(Student* student){
    printf("student %d: waking TA\n", student->studentId);
    waking_ta(student);
}

void wake_ta_next(Student* student){
    get_serviced(student);
}

void wait_in_queue(Student* student){
    printf("student %d: waiting in queue\n", student->studentId);
    return;
}

void wait_in_queue_next(Student* student){
    wait_until_called(student);
}

void receive_help(Student* student){
    printf("receiving %d: help\n", student->studentId);
    // wait for random period of time
    sleep(randnum(1, 5));
}

void receive_help_next(Student* student){
    dismiss_self(student);
}