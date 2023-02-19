#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> // for sleep()

/*
    Notes: Printing when TA is asleep is a bit difficult because the 
    TA thread sleeps when its helping a student outside of it's critical section, as it should.
    This means that a student thread can find the TA sleeping and wake him before
    the TA thread has printed that it's asleep. It really is asleep because its finished 
    helping all the students but the print statement hasn't executed yet.

*/

// Code for Queue to keep FIFO order
// This queue will store the student IDs which will then be used for semaphores for each student thread
struct Node {
    int data;
    struct Node* next;
};

struct Queue {
    struct Node* front;
    struct Node* rear;
    int count;
};

struct Queue* createQueue() {
    struct Queue* q = (struct Queue*) malloc(sizeof(struct Queue));
    q->front = NULL;
    q->rear = NULL;
    q->count = 0;
    return q;
}

void enqueue(struct Queue* q, int value) {
    struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->next = NULL;
    q->count++;
    if (q->rear == NULL) {
        q->front = newNode;
        q->rear = newNode;
        return;
    }

    q->rear->next = newNode;
    q->rear = newNode;
}

int peek(struct Queue* q) {
    if (q->front == NULL) {
        return -1;
    } 
    else {
        return q->front->data;
    }
}
int dequeue(struct Queue* q) {
    if (q->front == NULL) {
        return -1;
    }

    struct Node* temp = q->front;
    q->front = q->front->next;
    q->count--;

    if (q->front == NULL) {
        q->rear = NULL;
    }

    return temp->data;
    free(temp);
}

int count(struct Queue* q) {
    return q->count;
}

#define NUM_CHAIRS 1 // number of chairs in the hallway
#define PROGRAM_TIME 10 // time (in seconds) for each student to program
#define HELP_TIME 2 // time (in seconds) for the TA to help a student
#define NUM_STUDENTS 5 //number of students

pthread_mutex_t mutex; // mutex lock for shared data access
sem_t sem_ta; // semaphore for TA thread
int num_students_waiting = 0; // number of students waiting in the hallway, weirdly includes student currently being served
sem_t student_semaphores[NUM_STUDENTS]; //semaphore for each student thread, this ensures FIFO ordering
struct Queue* q; //Queue to keep track of which students to serve next
int currently_helping_student = 0;

void *ta_thread(void *arg) {

    printf("TA sleeping \n");
    while (1) {
        // wait for a student to wake up the TA
        sem_wait(&sem_ta);

        // acquire the mutex lock before accessing shared data
        pthread_mutex_lock(&mutex);

        int student_to_help = peek(q);
        printf("TA is helping student %d\n", student_to_help);

        // release the mutex lock after accessing shared data
        pthread_mutex_unlock(&mutex);

        sleep(rand() % HELP_TIME + 1);
        sem_post(&student_semaphores[student_to_help]); //TA finishes helping student

        pthread_mutex_lock(&mutex);
        dequeue(q);
        if(count(q) == 0) 
                printf("TA sleeping \n");
        pthread_mutex_unlock(&mutex);
    }   
}

void *student_thread(void *arg) {
    int student_id = *((int *) arg);
    while (1) {
        // program for a random amount of time
        printf("Student %d is programming...\n", student_id);
        sleep(rand() % PROGRAM_TIME + 1);

        // acquire the mutex lock before accessing shared data
        pthread_mutex_lock(&mutex);

        if (count(q) < NUM_CHAIRS + 1) {
            if(count(q) == 0)
                printf("Student %d finds TA sleeping and wakes him\n", student_id);
            else {
                printf("Student %d needs help but the TA is busy so student waits\n", student_id);
            }

            enqueue(q, student_id);
            pthread_mutex_unlock(&mutex);

            sem_post(&sem_ta);
            sem_wait(&student_semaphores[student_id]);
            printf("Student %d is done getting help from the TA...\n", student_id);
        } else {
            // otherwise, come back later
            printf("Student %d will try again later...\n", student_id);
            pthread_mutex_unlock(&mutex);
        }
    }
}

int main(int argc, char *argv[]) {
    // initialize the mutex lock and semaphores
    pthread_mutex_init(&mutex, NULL);
    sem_init(&sem_ta, 0, 0);

    for(int i = 0; i < NUM_STUDENTS; i++)
        sem_init(&student_semaphores[i], 0, 0);

    q = createQueue(); //keeps track of students to serve next

    // create the TA thread
    pthread_t ta;
    pthread_create(&ta, NULL, ta_thread, NULL);

    // create the student threads
    pthread_t students[NUM_STUDENTS];
    int student_ids[NUM_STUDENTS];
    for (int i = 0; i < NUM_STUDENTS; i++) {
        student_ids[i] = i;
        pthread_create(&students[i], NULL, student_thread, &student_ids[i]);
    }

    // join the threads, not strictly necessary since they run forever
    pthread_join(ta, NULL);

    for(int i = 0; i < NUM_STUDENTS; i++)
        pthread_join(students[i], NULL);

    return 0;
}