#include <pthread.h>
#include "context.h"
#include <stddef.h>

#define NUM_STUDENTS 5

pthread_t student[NUM_STUDENTS];
pthread_t ta;
pthread_attr_t attr;

void* student_fsm(void* param);
void* ta_fsm(void* param);

volatile int student_id_counter;

int main()
{
    pthread_mutex_t mutex;
    StudentQueue* student_queue = init_studentQueue(NUM_STUDENTS);
    Context* context = {NULL, student_queue, NULL, &mutex};

    student_id_counter = 0;

    pthread_mutex_init(&mutex, NULL);

    pthread_attr_init(&attr);
    pthread_create(&ta, &attr, ta_fsm, (void*)context);

	for(int i = 0; i < NUM_STUDENTS; i++) {		
		pthread_attr_init(&attr);
		pthread_create(&student[i], &attr, student_fsm, (void*)context);
	}
    
    pthread_join(ta, NULL);
    for(int i = 0; i < NUM_STUDENTS; i++){
		pthread_join(student[i], NULL);
    }

	return 0;
}

void* student_fsm(void* ctx) {

    Context* context = (Context*)ctx;

    sem_t queueSemphr;
    sem_init(&queueSemphr, 0, 1);
    //create a student
    Student student = {
        &NO_HELP_WANTED_STATE,
        context,
        &queueSemphr,
        student_id_counter++
    };
    while(1){
        //current state do action
        student.currentState->do_activity(&student);
        
        //current state do next action
        student.currentState->choose_next_state(&student);
    }

}

void* ta_fsm(void* ctx) {
    Context* context = (Context*)ctx;
    sem_t servicingSemphr;
    sem_t sleepingSemphr;
    sem_init(&servicingSemphr, 0, 1);
    sem_init(&sleepingSemphr, 0, 1);


    //create TA
    TeachingAssistant ta = {
        &SLEEPING_STATE, 
        context, 
        &servicingSemphr, 
        &sleepingSemphr
    };

    while(1){
        //current state do action
        ta.currentState->do_activity(&ta);
        
        //current state do next action
        ta.currentState->choose_next_state(&ta);
    }
}


/*

void* students_fsm(void* param) {
	int start = ((parameters*)param)->start;
	int end = ((parameters*)param)->end;
	
	for(int i = start; i < end; i++) {
		sum += sum_array[i];
	}

	pthread_exit(0);
}

void* ta_fsm(void* param) {
	int start = ((parameters*)param)->start;
	int end = ((parameters*)param)->end;
	
	for(int i = start; i < end; i++) {
		sum += sum_array[i];
	}

	pthread_exit(0);
} */