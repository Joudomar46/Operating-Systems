#include <stdlib.h>
#include <stdio.h>
#include "buffer_mon.h"
#include <string.h>
#include <pthread.h>


static ring_buffer_421_t buffer;
static pthread_mutex_t mutex;         //lock but shared by multiple processes(threads) block R/W when node is being used
static pthread_cond_t fill_count;        //conditional variable for enqueue
static pthread_cond_t empty_count;       //conditional variable for dequeue

long init_buffer_421(void) {
    //buffer = NULL no buffer
    if(buffer.read == NULL) {
        node_421_t *cursor = NULL;
        buffer.length = 0;                              //will fill this in as we write data
        buffer.write = NULL;
        buffer.read = NULL;

        //initialize monitor
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&fill_count, NULL);
        pthread_cond_init(&empty_count, NULL);

        //create 20 Nodes for buffer
        for (int i = 0; i < SIZE_OF_BUFFER; i++) {
            struct node_421 *newNode = malloc(sizeof(struct node_421));
            newNode->next = NULL;   //for now

            //two cases
            //is it the first node created
            if (cursor == NULL) {
                newNode->next = newNode;                //point to itself
                cursor = newNode;
            } else {
                newNode->next = cursor->next;       //the next of new item will point to front of queue
                cursor->next = newNode;             //the current pointer to front will point to new item
                cursor = cursor->next;              //advance the new cursor
            }
        }
        //after filling the buffer make:
        buffer.read = cursor;                          //read will act as global tail pointing to head
        if (buffer.read->next != NULL) {               //write cursor points at the first element
            buffer.write = buffer.read->next;          //write will act as current cursor for information
        }
        return 0;
    }
    //under any other circumstance error
    fprintf(stderr, "\nBuffer_user.c Error while initializing buffer.\n");
    return -1;
}

long enqueue_buffer_421(char * data) {
    //is Buffer initialized?
    if (buffer.read != NULL) {

        pthread_mutex_unlock(&mutex);           //thread gets mutex lock once other thread finishes
        while(buffer.length >= SIZE_OF_BUFFER){  //thread waits if full buffer aka empty reached 0 from dequeue: BLOCKS
        pthread_cond_wait( &empty_count, &mutex );
        }

        if (buffer.length != SIZE_OF_BUFFER) {                //if not full
            //insert next available node, copy each char into write->data char array
            for(int i = 0; i < DATA_LENGTH; i++){
                buffer.write->data[i] = data[i];
            }

            printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
            printf("Enqueue length pre-update: %d\n", buffer.length);
            printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

            buffer.write = buffer.write->next;             //advance the new cursor
            buffer.length++;                               //Increment the buffer length when you insert.

            pthread_cond_signal(&fill_count);        //thread signals the fill count condition
            pthread_mutex_unlock(&mutex);           //thread releases mutex lock once it is finishes

            return 0;
        }
    }
    //under any other circumstance error
    fprintf(stderr, "\nBuffer_user.c Error enqueuing into buffer.\n");
    return -1;
}

long dequeue_buffer_421(char * data) {
    //check if buffer is initialized
    if (buffer.read != NULL) {

        pthread_mutex_unlock(&mutex);           //thread gets mutex lock once other thread finishes
        while(buffer.length <= 0){  //thread waits if empty buffer aka fill reached 0 from enqueue: BLOCKS
            pthread_cond_wait( &fill_count, &mutex );
        }

        if (buffer.length > 0) {                                //if not empty
            //read next available node, copy each char into pointer data char array
            for(int i = 0; i < DATA_LENGTH; i++){
                data[i]= buffer.read->next->data[i];            //since read starts at the tail, must go to the head to read
            }

            buffer.read = buffer.read->next;             //advance the cursor
            buffer.length--; //Increment the buffer length when you insert.

            printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
            printf("Dequeue length post-update:: %d\n", buffer.length);
            printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

            pthread_cond_signal(&empty_count);        //thread signals the fill count condition
            pthread_mutex_unlock(&mutex);           //thread releases mutex lock once it is finishes

            return 0;
        }
    }
    //under any other circumstance error
    fprintf(stderr, "\nBuffer_user.c Error dequeuing buffer.\n");
    return -1;
}

long delete_buffer_421(void) {
    // Tip: Don't call this while any process is waiting to enqueue or dequeue.
    // write your code to delete buffer and other unwanted components
    // is Buffer initialized?
    if (buffer.read != NULL) {
        node_421_t *temp;; //so we can delete the malloc objects
        node_421_t *end = buffer.read;    //to keep track of where to stop but its circular
        // so just set current read node as the "end"
        buffer.read = buffer.read->next;   //start deleting at the "start" of linked list

        while (buffer.read!= end)        //until I reach the last node
        {
            temp = buffer.read;
            buffer.read = buffer.read->next;    //go to next to be deleted
            free(temp);                             //delete current
        }
        //reset newBuff
        buffer.read = NULL;
        buffer.write = NULL;
        buffer.length = 0;

        //free the monitor
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&fill_count);
        pthread_cond_destroy(&empty_count);

        free(end);                                  //delete last node

        return 0;
    }
    fprintf(stderr, "\nBuffer_user.c Error deleting buffer.\n");
    return -1;
}
