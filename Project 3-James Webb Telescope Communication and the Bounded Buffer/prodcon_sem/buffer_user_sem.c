#include <stdlib.h>
#include <stdio.h>
#include "buffer_sem.h"
#include <string.h>

static bb_buffer_421_t buffer;
static sem_t mutex;             //lock but shared by multiple processes(threads) block R/W when node is being used
static sem_t fill_count;        //semaphore for enqueue
static sem_t empty_count;       //semaphore for dequeue

long init_buffer_421(void) {
    //buffer = NULL no buffer
    if (buffer.read == NULL) {
        bb_node_421_t *cursor = NULL;
        buffer.length = 0;                              //will fill this in as we write data
        buffer.write = NULL;
        buffer.read = NULL;

        //initialize semaphore
        sem_init(&mutex, 0, 1);                           //lock begins at 1
        sem_init(&fill_count, 0, 0);                      //fill starts at 0 grows each time enqueue is executed
        sem_init(&empty_count, 0, SIZE_OF_BUFFER);        //empty starts at size of buffer 20, decreases with dequeue

        //create 20 Nodes for buffer
        for (int i = 0; i < SIZE_OF_BUFFER; i++) {
            struct bb_node_421 *newNode = malloc(sizeof(struct bb_node_421));
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


long enqueue_buffer_421(char *data) {
    //is Buffer initialized?
    if (buffer.read != NULL) {

        sem_wait(&empty_count);                 //thread waits if full buffer aka empty reached 0 from dequeue: BLOCKS
        sem_wait(&mutex);                       //thread waits for mutex lock once other thread finishes

        if (buffer.length != SIZE_OF_BUFFER) {                //if not full
            //insert next available node, copy each char into write->data char array
            for (int i = 0; i < DATA_LENGTH; i++) {
                buffer.write->data[i] = data[i];
            }

            printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
            print_semaphores();
            printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

            buffer.write = buffer.write->next;             //advance the new cursor
            buffer.length++;                               //Increment the buffer length when you insert.

            sem_post(&mutex);                       //thread releases mutex lock once it is finishes
            sem_post(&fill_count);                  //thread increments the fill count

            return 0;
        }
    }
    //under any other circumstance error
    fprintf(stderr, "\nBuffer_user.c Error enqueuing into buffer.\n");
    return -1;
}

long dequeue_buffer_421(char *data) {
    //check if buffer is initialized
    if (buffer.read != NULL) {

        sem_wait(&fill_count);                  //thread waits if empty buffer aka fill reached 0 from enqueue: BLOCKS
        sem_wait(&mutex);                       //thread waits for mutex lock once other thread finishes

        if (buffer.length > 0) {                                //if not empty
            //read next available node, copy each char into pointer data char array
            for (int i = 0; i < DATA_LENGTH; i++) {
                data[i] = buffer.read->next->data[i];            //since read starts at the tail, must go to the head to read
            }

            buffer.read = buffer.read->next;             //advance the cursor
            buffer.length--; //Increment the buffer length when you insert.

            printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
            print_semaphores();
            printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

            sem_post(&mutex);                        //thread releases mutex lock once it is finishes
            sem_post(&empty_count);                  //thread increments the empty count

            return 0;
        }
    }
    //under any other circumstance error
    fprintf(stderr, "\nBuffer_user.c Error dequeuing buffer.\n");
    return -1;
}


long delete_buffer_421(void) {
    // Tip: Don't call this while any process is waiting to enqueue or dequeue.
    //is Buffer initialized?
    if (buffer.read != NULL) {
        bb_node_421_t *temp;; //so we can delete the malloc objects
        bb_node_421_t *end = buffer.read;    //to keep track of where to stop but its circular
        // so just set current read node as the "end"
        buffer.read = buffer.read->next;   //start deleting at the "start" of linked list

        while (buffer.read != end)        //until I reach the last node
        {
            temp = buffer.read;
            buffer.read = buffer.read->next;    //go to next to be deleted
            free(temp);                             //delete current
        }
        //reset newBuff
        buffer.read = NULL;
        buffer.write = NULL;
        buffer.length = 0;

        //free the semaphores
        sem_destroy(&mutex);
        sem_destroy(&fill_count);
        sem_destroy(&empty_count);

        free(end);                                  //delete last node

        return 0;
    }
    fprintf(stderr, "\nBuffer_user.c Error deleting buffer.\n");
    return -1;
}

void print_semaphores(void) {
    // You can call this method to check the status of the semaphores.
    // Don't forget to initialize them first!
    // YOU DO NOT NEED TO IMPLEMENT THIS FOR KERNEL SPACE.
    int value;
    sem_getvalue(&mutex, &value);
    printf("sem_t mutex = %d\n", value);
    sem_getvalue(&fill_count, &value);
    printf("sem_t fill_count = %d\n", value);
    sem_getvalue(&empty_count, &value);
    printf("sem_t empty_count = %d\n", value);
    return;
}
