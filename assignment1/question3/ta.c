#include "ta.h"

void help_student(TeachingAssistant* ta){
    // call next student
    ta->queue->callNextStudent(ta->queue);

    // todo: help student for random time
}

void help_student_next(TeachingAssistant* ta){
    if(!ta->queue->hasStudent(ta->queue)){
        ta->currentState = HELPING_STUDENT_STATE;
    } else ta->currentState = SLEEPING_STATE;
}

void sleep(TeachingAssistant* ta){
    return;
}

void sleep_next(TeachingAssistant* ta){
    // todo: take smphr

    // transitition to STATE_HELPING_STUDENT
    ta->currentState = HELPING_STUDENT_STATE;
}

HELPING_STUDENT_STATE = {
    STATE_HELPING_STUDENT,
    help_student,
    help_student_next
};

SLEEPING_STATE = {
    STATE_SLEEPING,
    sleep,
    sleep_next,
};
