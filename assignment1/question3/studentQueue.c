#include "context.h"

#include <stddef.h>
#include <stdio.h>

// todo: write queue functionality
StudentQueue* init_studentQueue(int maxSize){
    StudentQueue* queue = malloc(sizeof(StudentQueue));
    queue->front = 0;
    queue->rear = 0;
    queue->count = 0;
    queue->maxSize = maxSize;
    queue->queue = malloc(maxSize * sizeof(Student*));

    return queue;
}

int isFull(StudentQueue* queue){
    return queue->count == queue->maxSize;
}

int isEmpty(StudentQueue* queue){
    return queue->count == 0;
}

Student* peek(StudentQueue* queue){
    return queue->queue[queue->front];
}

int enqueue(StudentQueue* queue, Student* student){
    //printf("queue: %d student waiting (max allowed = %d)", queue->count, queue->maxSize);
    if(isFull(queue)){
        return 0;
    }

    queue->queue[queue->rear] = student;
    queue->rear = (queue->rear + 1) % queue->maxSize;
    queue->count++;
    return 1;
}

Student* dequeue(StudentQueue* queue){
    if(isEmpty(queue)){
        return NULL;
    }
    
    Student* rslt = peek(queue);
    queue->front = (queue->front + 1) % queue->maxSize;
    queue->count--;

    return rslt;
}