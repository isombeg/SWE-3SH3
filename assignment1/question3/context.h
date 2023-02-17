#ifndef CONTEXT_H
#define CONTEXT_H

#include "ta.h"

typedef struct __Context__ {
    TeachingAssistant ta;

    int (*isTaBusy)(Context* context);
} Context;

#endif