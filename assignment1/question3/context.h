#ifndef CONTEXT_H
#define CONTEXT_H

#include <semaphore.h>
#include <stdlib.h>

#define randnum(min, max) ((rand() % (int)(((max) + 1) - (min))) + (min))

typedef struct __Context__ Context;
typedef struct __Student__ Student;
typedef struct __TeachingAssistant__ TeachingAssistant;
typedef struct __StudentQueue__ StudentQueue;
typedef struct __TaState__ TaState;
typedef struct __StudentState__ StudentState;

typedef struct __Context__ {
    TeachingAssistant* ta;
    StudentQueue* queue;
    Student* currentServicedStudent;
    pthread_mutex_t* mutex;
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

/* ---------------------------- TA ---------------------------------*/

typedef enum {
    STATE_HELPING_STUDENT,
    STATE_SLEEPING
} TA_STATE_ID;

// typedef struct __TeachingAssistant__ TeachingAssistant;
// typedef struct __TaState__ TaState;

typedef struct __TeachingAssistant__ {
    TaState* currentState;
    Context* context;
    sem_t* servicingSemphr; // is signal for being dismissed from servicing
    sem_t* sleepingSemphr; // is signal for waking up
} TeachingAssistant;

typedef struct __TaState__ {
    TA_STATE_ID stateId;

    void (*do_activity)(TeachingAssistant* ta);
    void (*choose_next_state)(TeachingAssistant* ta);
} TaState;

TaState* HELPING_STUDENT_STATE;
void help_student(TeachingAssistant* ta);
void help_student_next(TeachingAssistant* ta);

TaState* SLEEPING_STATE;
void ta_go_to_sleep();
void ta_go_to_sleep_next();

/* ---------------------------- Student ---------------------------------*/

typedef enum {
    STATE_NO_HELP_WANTED,
    STATE_WAKING_TA,
    STATE_WAITING_IN_QUEUE,
    STATE_RECEIVING_HELP,
} STUDENT_STATE_ID;

typedef struct __Student__ Student;
typedef struct __StudentState__ StudentState;

struct __Student__ {
    StudentState* currentState;
    Context* context;
    sem_t* queueSemphr;
    int studentId;
};

struct __StudentState__ {
    STUDENT_STATE_ID stateId;

    void (*do_activity)(Student* student);
    void (*choose_next_state)(Student* student);
};

StudentState* NO_HELP_WANTED_STATE;
void program(Student* student);
void program_next(Student* student);

StudentState* WAKING_TA_STATE;
void wake_ta(Student* student);
void wake_ta_next(Student* student);

StudentState* WAITING_IN_QUEUE_STATE;
void wait_in_queue(Student* student);
void wait_in_queue_next(Student* student);

StudentState* RECEIVING_HELP_STATE;
void receive_help(Student* student);
void receive_help_next(Student* student);

/* ---------------------------- StudentQueue ---------------------------------*/

typedef struct __StudentQueue__ {
    int front;
    int rear;
    int count;
    int maxSize;
    Student** queue;
} StudentQueue;

StudentQueue* init_studentQueue(int maxSize);
int isFull(StudentQueue* queue);
int isEmpty(StudentQueue* queue);
Student* peek(StudentQueue* queue);
int enqueue(StudentQueue* queue, Student* student);
Student* dequeue(StudentQueue* queue);

#endif