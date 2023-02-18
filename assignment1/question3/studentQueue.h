#ifndef STUDENTQUEUE_H
#define STUDENTQUEUE_H

#include "student.h"

typedef struct {
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