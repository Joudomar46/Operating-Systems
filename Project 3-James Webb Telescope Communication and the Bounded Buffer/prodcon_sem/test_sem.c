#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include "buffer_sem.h"

#define __NR_init_buffer_sem_421 446
long init_buffer_sem_421_syscall(void) {
    return syscall(__NR_init_buffer_sem_421);
}
#define __NR_enqueue_buffer_sem_421 447
long enqueue_buffer_sem_421_syscall(char *data) {
    return syscall(__NR_enqueue_buffer_sem_421, data);
}
#define __NR_dequeue_buffer_sem_421 448
long dequeue_buffer_sem_421_syscall(char *data) {
    return syscall(__NR_dequeue_buffer_sem_421, data);
}
#define __NR_delete_buffer_sem_421 449
long delete_buffer_sem_421_syscall(void) {
    return syscall(__NR_delete_buffer_sem_421);
}

#define TEST_RUN_LOOP 1000

void *producer_enqueue(void *arg){
    srand((unsigned) 1934);             //for random seed

    int fillChar = 0;                   //for looping enqueue chars from 0 to 9
    float randTime;                     //to insert into sleep function to simulate unpredictable internet data stream
    int returnCounter = 0;              //to track how many times dequeue returned 0

    for(int j = 0; j < TEST_RUN_LOOP; j++){                                 //start 1000 test trail iteration

        randTime = rand()%10;                                               //random number from 0-10
        randTime = randTime/1000;                                           //1 second = 0.001 millisecond
        sleep(randTime);                                                    //sleepy time before starting the thread operation

        char str[DATA_LENGTH] = {0};                                              //temp char array to fill with data to feed enqueue
        for(int fill = 0; fill < DATA_LENGTH-1;fill++){                   //fill loop to have 000000...111111...22222 char array
            str[fill] =  fillChar + '0';
        }
        str[DATA_LENGTH] =  '\0';   //NULL terminated character
        fillChar++;                 //iterate next number to fill
        if(fillChar == 10){         //want 0 to 9
            fillChar = 0;           //reset insertion back to 0
        }
        printf("Produced: %c\n", str[0]);

        if(!enqueue_buffer_sem_421_syscall(str)){       //update returnCounter when enqueue returns 0
            returnCounter++;
        }else{
            break;
        }
    }

    if(returnCounter == TEST_RUN_LOOP){
        printf("Producer thread ran successfully for the %d times.\n",TEST_RUN_LOOP);
    }else{
        printf("Producer thread error and ran %d times successfully.\n",returnCounter);
    }
    return NULL;
}

void *consumer_dequeue(void *arg){
    srand((unsigned)493842);                //for random seed
    float randTime;                         //to insert into sleep function to simulate unpredictable internet data stream
    int returnCounter = 0;                  //to track how many times dequeue returned 0

    for(int j = 0; j < TEST_RUN_LOOP; j++){             //start 1000 test trail iteration
        randTime = rand()%10;                           //random number from 0-10
        randTime = randTime/1000;                       //1 second = 0.001 millisecond
        sleep(randTime);                     //sleepy time before starting the thread operation

        char data[DATA_LENGTH] = {0};         //pass char array to dequeue
        if(!dequeue_buffer_sem_421_syscall(data)){       //update returnCounter when dequeue returns 0
            returnCounter++;
        }else{
            break;
        }
        printf("Consumed: ");
        printf("%c\n",data[1]);
    }

    if(returnCounter == TEST_RUN_LOOP){
        printf("Consumer thread ran successfully for the %d times.\n",TEST_RUN_LOOP);
    }else{
        printf("Consumer thread error and ran %d times successfully.\n",returnCounter);
    }
    return NULL;
}

int main() {
    //Biggest test, threads enqueuing and dequeue 1000 times.
    pthread_t producer, consumer;               //create two threads
    if (init_buffer_sem_421_syscall() == 0) {
        pthread_create(&producer, NULL, producer_enqueue, NULL);        //pass thread to function above
        pthread_create(&consumer, NULL, consumer_dequeue, NULL);        //pass thread to function above

        pthread_join(producer, NULL);               //waiting for threads to finish before exit
        pthread_join(consumer, NULL);               //waiting for threads to finish before exit
        printf("Buffer initialized correctly and threading returned...\n");
        if (init_buffer_sem_421_syscall() != 0){
            printf("Initialized while buffer already initialized test passed...\n");
        }else{
            printf("Initialized while buffer already initialized test failed...\n");
        }
        printf("Deleting buffer...\n");
        delete_buffer_sem_421_syscall();
    }
    printf("\nenqueue or dequeue while buffer not initialized...\n");
    pthread_create(&producer, NULL, producer_enqueue, NULL);        //pass thread to function above
    pthread_create(&consumer, NULL, consumer_dequeue, NULL);        //pass thread to function above
    pthread_join(producer, NULL);               //waiting for threads to finish before exit
    pthread_join(consumer, NULL);               //waiting for threads to finish before exit

    printf("\nDeleting a non initialized buffer...\n");
    if (delete_buffer_sem_421_syscall() != 0) {
        printf("Delete while buffer not initialized test passed...\n");
    }else{
        printf("Delete while buffer not initialized failed passed...\n");
    }

    printf("\nInitialize and delete a empty buffer...\n");
    if (init_buffer_sem_421_syscall() == 0){
        printf("Initialized while buffer already initialized test passed...\n");
    }else{
        printf("Initialized while buffer already initialized test failed...\n");
    }

    if (delete_buffer_sem_421_syscall() == 0) {
        printf("Delete while buffer not initialized test passed...\n");
    }else{
        printf("Delete while buffer not initialized failed passed...\n");
    }

    return 0;
}
