#ifndef STUDENTQUEUE_H
#define STUDENTQUEUE_H

#include "student.h"

typedef struct {
    int front;
    int rear;
    int maxSize;
    Student* queue;

    int (*hasStudent)(StudentQueue* queue);
    Student (*callNextStudent)(StudentQueue* queue);
} StudentQueue;

#endif