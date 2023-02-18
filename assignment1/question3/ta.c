#include "context.h"

#include <stdio.h>

void help_student(TeachingAssistant* ta){
    printf("ta: helping students\n");
    wait_for_dismissal(ta);
}

void help_student_next(TeachingAssistant* ta){
    if(are_students_waiting(ta)){
        call_next_student(ta);
    } else go_to_sleep(ta);
}

void sleep(TeachingAssistant* ta){
    printf("ta: sleeping\n");
    return;
}

void sleep_next(TeachingAssistant* ta){
    sleep_til_woken(ta);
}