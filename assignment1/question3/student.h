#ifndef STUDENT_H
#define STUDENT_H

#include "context.h"

typedef enum {
    STATE_NO_HELP_WANTED,
    STATE_WAKING_TA,
    STATE_WAITING_IN_QUEUE,
    STATE_RECEIVING_HELP,
} STUDENT_STATE_ID;

typedef struct __StudentState__ {
    STUDENT_STATE_ID stateId;

    void (*do_activity)(Student* student);
    void (*choose_next_state)(Student* student);
} StudentState;

extern const StudentState NO_HELP_WANTED_STATE;
void program(Student* student);
void program_next(Student* student);

extern const StudentState WAKING_TA_STATE;
void wake_ta(Student* student);
void wake_ta_next(Student* student);

extern const StudentState WAITING_IN_QUEUE_STATE;
void wait_in_queue(Student* student);
void wait_in_queue_next(Student* student);

extern const StudentState RECEIVING_HELP_STATE;
void receive_help(Student* student);
void receive_help_next(Student* student);

typedef struct {
    StudentState* currentState;
    Context* context;

    int (*check_if_ta_busy)(Student* student);
    int (*enter_queue)(Student* student);
} Student;

#endif