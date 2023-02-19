#include "context.h"

#include <semaphore.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>

#define lock() pthread_mutex_lock(context->mutex)
#define unlock() pthread_mutex_unlock(context->mutex)
// todo: wrap all race critical ops in mutexes

int is_TaBusy(Context* context){
    lock();
    int isTaBusy = context->ta->currentState == HELPING_STUDENT_STATE;
    unlock();
    return isTaBusy;
}

void try_entering_queue(Student* student){
    int enteredQueue;
    Context* context = student->context;
    
    if(is_TaBusy(context)){
        //printf("ta was busy\n");
        lock();
        enteredQueue = enqueue(context->queue, student);
        unlock();
        if(enteredQueue){
            //printf("Entered queue\n");
            student->currentState = WAITING_IN_QUEUE_STATE;
        } 
        else {
            //printf("couldn't enter queue\n");
            student->currentState = NO_HELP_WANTED_STATE;
        }
    } else {
        //printf("ta was not busy\n");
        student->currentState = WAKING_TA_STATE;
    }
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
    Context* context = student->context;
    lock();
    student->context->currentServicedStudent = student;
    unlock();
    student->currentState = RECEIVING_HELP_STATE;
}

void dismiss_self(Student* student){
    Context* context = student->context;
    lock();
    student->context->currentServicedStudent = NULL;
    unlock();

    student->currentState = NO_HELP_WANTED_STATE;
    sem_post(student->context->ta->servicingSemphr);
}

void call_next_student(TeachingAssistant* ta){
    Context* context = ta->context;
    lock();
    Student* nextStudent = dequeue(ta->context->queue);
    unlock();

    sem_post(nextStudent->queueSemphr);
}

void wait_for_dismissal(TeachingAssistant* ta){
    sem_wait(ta->servicingSemphr);
}

int are_students_waiting(TeachingAssistant* ta){
    return !isEmpty(ta->context->queue);
}

void go_to_sleep(TeachingAssistant* ta){
    ta->currentState = SLEEPING_STATE;
}

void sleep_til_woken(TeachingAssistant* ta){
    sem_wait(ta->sleepingSemphr);

    ta->currentState = HELPING_STUDENT_STATE;
}