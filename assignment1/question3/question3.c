#include <pthread.h>
#include "context.h"
#include <stddef.h>
#include <stdio.h>

#define NUM_STUDENTS 4
#define MAX_STUDENTS 3

pthread_t student[NUM_STUDENTS];
pthread_t ta;
pthread_attr_t attr;

void* student_fsm(void* param);
void* ta_fsm(void* param);

volatile int student_id_counter;

int main()
{
    pthread_mutex_t mutex;
    StudentQueue* student_queue = init_studentQueue(MAX_STUDENTS);
    Context context = {NULL, student_queue, NULL, &mutex};

    // Initialize states
    HELPING_STUDENT_STATE = malloc(sizeof(TaState));
    HELPING_STUDENT_STATE->stateId = STATE_HELPING_STUDENT;
    HELPING_STUDENT_STATE->do_activity = help_student;
    HELPING_STUDENT_STATE->choose_next_state = help_student_next;

    SLEEPING_STATE = malloc(sizeof(TaState));
    SLEEPING_STATE->stateId = STATE_SLEEPING;
    SLEEPING_STATE->do_activity = ta_go_to_sleep;
    SLEEPING_STATE->choose_next_state = ta_go_to_sleep_next;

    NO_HELP_WANTED_STATE = malloc(sizeof(StudentState));
    NO_HELP_WANTED_STATE->stateId = STATE_NO_HELP_WANTED;
    NO_HELP_WANTED_STATE->do_activity = program;
    NO_HELP_WANTED_STATE->choose_next_state = program_next;

    WAKING_TA_STATE = malloc(sizeof(StudentState));
    WAKING_TA_STATE->stateId = STATE_WAKING_TA;
    WAKING_TA_STATE->do_activity = wake_ta;
    WAKING_TA_STATE->choose_next_state = wake_ta_next;

    WAITING_IN_QUEUE_STATE = malloc(sizeof(StudentState));
    WAITING_IN_QUEUE_STATE->stateId = STATE_WAITING_IN_QUEUE;
    WAITING_IN_QUEUE_STATE->do_activity = wait_in_queue;
    WAITING_IN_QUEUE_STATE->choose_next_state = wait_in_queue_next;

    RECEIVING_HELP_STATE = malloc(sizeof(StudentState));
    RECEIVING_HELP_STATE->stateId = STATE_RECEIVING_HELP;
    RECEIVING_HELP_STATE->do_activity = receive_help;
    RECEIVING_HELP_STATE->choose_next_state = receive_help_next;

    student_id_counter = 0;

    pthread_mutex_init(&mutex, NULL);

    pthread_attr_init(&attr);
    pthread_mutex_lock(&mutex);
    pthread_create(&ta, &attr, ta_fsm, (void*)&context);

	for(int i = 0; i < NUM_STUDENTS; i++) {		
		pthread_attr_init(&attr);
		pthread_create(&student[i], &attr, student_fsm, (void*)&context);
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
        NO_HELP_WANTED_STATE,
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
    printf("creating ta thread\n");
    Context* context = (Context*)ctx;
    printf("here\n");
    sem_t servicingSemphr;
    sem_t sleepingSemphr;
    sem_init(&servicingSemphr, 0, 1);
    sem_init(&sleepingSemphr, 0, 1);


    //create TA
    TeachingAssistant* ta = malloc(sizeof(TeachingAssistant));
    ta->currentState = SLEEPING_STATE;
    ta->context = context;
    ta->servicingSemphr = &servicingSemphr;
    ta->sleepingSemphr = &sleepingSemphr;

    context->ta = ta;
    pthread_mutex_unlock(context->mutex);

    while(1){
        //current state do action
        ta->currentState->do_activity(ta);
        
        //current state do next action
        ta->currentState->choose_next_state(ta);
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