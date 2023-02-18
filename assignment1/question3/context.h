#ifndef CONTEXT_H
#define CONTEXT_H

#include "ta.h"
#include "student.h"

typedef struct __Context__ {
    TeachingAssistant* ta;
    StudentQueue* queue;
    Student* currentServicedStudent;
} Context;

int is_TaBusy(Context* context);

/* Student state control */

// Is the guard for Student::NO_HELP_WANTED
void try_entering_queue(Student* student);

// Consumes semphr to know when to stop waiting
void wait_until_called(Student* student); 

// Wakes the TA and enters STATE_RECEIVING_HELP
void waking_ta(Student* student);

// Changes student's state to RECEIVING_HELP
void get_serviced(Student* student);

// Changes student's state to NO_HELP_WANTED
void dismiss_self(Student* student);

/* TA state control */

// Retrieves next student from queue and enter STATE_HELPING_STUDENT
void call_next_student(TeachingAssistant* ta);

// Wait for student to dismiss TA
void wait_for_dismissal(TeachingAssistant* ta);

// Check if a student is waiting
int are_students_waiting(TeachingAssistant* ta);

// TA goes to sleep
void go_to_sleep(TeachingAssistant* ta);

// Is the guard after STATE_SLEEPING
void sleep_til_woken(TeachingAssistant* ta);

#endif