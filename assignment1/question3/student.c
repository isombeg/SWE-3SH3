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

    student->context->isTaBusy;
}

void program_next(Student* student){
    int entered_queue;

    if(student->check_if_ta_busy(student)){
        entered_queue = student->enter_queue(student);
        if(entered_queue) student->currentState = WAITING_IN_QUEUE_STATE;
        else student->currentState = NO_HELP_WANTED_STATE;
    } else student->currentState = WAKING_TA_STATE;
}

