#include "ta.h"

void help_student(TeachingAssistant* ta){
    wait_for_dismissal(ta);
}

void help_student_next(TeachingAssistant* ta){
    if(are_students_waiting(ta)){
        call_next_student(ta);
    } else go_to_sleep(ta);
}

void sleep(TeachingAssistant* ta){
    return;
}

void sleep_next(TeachingAssistant* ta){
    sleep_til_woken(ta);
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
