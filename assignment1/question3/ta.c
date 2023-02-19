#include "context.h"

#include <stdio.h>

void help_student(TeachingAssistant* ta){
    //printf("ta: helping students\n");
    wait_for_dismissal(ta);
}

void help_student_next(TeachingAssistant* ta){
    //printf("ta: dismissed\n");
    if(are_students_waiting(ta)){
        call_next_student(ta);
    } else go_to_sleep(ta);
}

void ta_go_to_sleep(TeachingAssistant* ta){
    //printf("ta: sleeping\n");
    return;
}

void ta_go_to_sleep_next(TeachingAssistant* ta){
    //printf("ta: waiting to be woken up\n");
    sleep_til_woken(ta);
}