#ifndef TA_H
#define TA_H

#include "studentQueue.h"

typedef enum {
    STATE_HELPING_STUDENT,
    STATE_SLEEPING
} TA_STATE_ID;

typedef struct __TaState__ {
    TA_STATE_ID stateId;

    void (*do_activity)(TeachingAssistant* ta);
    void (*choose_next_state)(TeachingAssistant* ta);
} TaState;

extern const TaState HELPING_STUDENT_STATE;
void help_student(TeachingAssistant* ta);
void help_student_next(TeachingAssistant* ta);

extern const TaState SLEEPING_STATE;
void sleep();
void sleep_next();

typedef struct __TeachingAssistant__ {
    TaState currentState;
    StudentQueue* queue;
} TeachingAssistant;

#endif