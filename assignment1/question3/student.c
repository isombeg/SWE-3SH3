#include "student.h"

NO_HELP_WANTED_STATE = {
    STATE_NO_HELP_WANTED,
    program,
    program_next
};

WAKING_TA_STATE = {
    STATE_WAKING_TA,
    wake_ta,
    wake_ta_next
};

WAITING_IN_QUEUE_STATE = {
    STATE_WAITING_IN_QUEUE,
    wait_in_queue,
    wait_in_queue_next
};

RECEIVING_HELP_STATE = {
    STATE_RECEIVING_HELP,
    receive_help,
    receive_help_next
};

void program(Student* student){
    // todo: wait random amount of time
}

void program_next(Student* student){
    try_entering_queue(student);
}

void wake_ta(Student* student){
    waking_ta(student);
}

void wake_ta_next(Student* student){
    get_serviced(student);
}

void wait_in_queue(Student* student){
    return;
}

void wait_in_queue_next(Student* student){
    wait_until_called(student);
}

void receive_help(Student* student){
    // todo: wait for random period of time
}

void receive_help_next(Student* student){
    dismiss_self(student);
}