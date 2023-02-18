#include "context.h"

#include <semaphore.h>
#include <stddef.h>

// todo: wrap all race critical ops in mutexes

int is_TaBusy(Context* context){
    return context->ta->currentState == &HELPING_STUDENT_STATE;
}

void try_entering_queue(Student* student){
    int enteredQueue;

    if(is_TaBusy(student->context)){
        enteredQueue = enqueue(student->context->queue, student);
        if(enteredQueue) student->currentState = &WAITING_IN_QUEUE_STATE;
        else student->currentState = &NO_HELP_WANTED_STATE;
    } else student->currentState = &WAKING_TA_STATE;
}

void wait_until_called(Student* student){
    sem_wait(student->queueSemphr);
    get_serviced(student);
    return;
}

void waking_ta(Student* student){
    sem_post(student->context->ta->sleepingSemphr);
}

void get_serviced(Student* student){
    student->context->currentServicedStudent = student;
    student->currentState = &RECEIVING_HELP_STATE;
}

void dismiss_self(Student* student){
    student->context->currentServicedStudent = NULL;
    student->currentState = &NO_HELP_WANTED_STATE;

    sem_post(student->context->ta->servicingSemphr);
}

void call_next_student(TeachingAssistant* ta){
    Student* nextStudent = dequeue(ta->context->queue);
    sem_post(nextStudent->queueSemphr);
}

void wait_for_dismissal(TeachingAssistant* ta){
    sem_wait(ta->servicingSemphr);
}

int are_students_waiting(TeachingAssistant* ta){
    return !isEmpty(ta->context->queue);
}

void go_to_sleep(TeachingAssistant* ta){
    ta->currentState = &SLEEPING_STATE;
}

void sleep_til_woken(TeachingAssistant* ta){
    sem_wait(ta->sleepingSemphr);

    ta->currentState = &HELPING_STUDENT_STATE;
}