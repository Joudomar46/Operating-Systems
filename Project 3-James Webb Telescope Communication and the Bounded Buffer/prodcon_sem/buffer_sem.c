#include "buffer_sem.h"
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/slab.h>

static bb_buffer_421_t buffer;
static struct semaphore mutex;             //lock but shared by multiple processes(threads) block R/W when node is being used
static struct semaphore fill_count;        //semaphore for enqueue
static struct semaphore empty_count;       //semaphore for dequeue

SYSCALL_DEFINE0(init_buffer_sem_421){
    //DECLARATIONS FOR THE KERNEL UGHHHH
    bb_node_421_t *cursor;
    int i;
    bb_node_421_t *newNode;
    //buffer = NULL no buffer
    if(buffer.read == NULL) {
        cursor = NULL;
        buffer.length = 0;                              //will fill this in as we write data
        buffer.write = NULL;
        buffer.read = NULL;

        //initialize semaphore
        sema_init(&mutex,1);                           //lock begins at 1
        sema_init(&fill_count,0);                      //fill starts at 0 grows each time enqueue is executed
        sema_init(&empty_count,SIZE_OF_BUFFER);        //empty starts at size of buffer 20, decreases with dequeue

        //create 20 Nodes for buffer
        for (i = 0; i < SIZE_OF_BUFFER; i++) {
            newNode = kmalloc(sizeof(struct bb_node_421), GFP_KERNEL);
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
    printk("\nBuffer.c Error while initializing buffer.\n");
    return -1;
}

SYSCALL_DEFINE1(enqueue_buffer_sem_421, char*, data) {
    int is_copy;                            //check return of copy
    //is Buffer initialized?
    if (buffer.read != NULL) {
        down(&empty_count);                 //thread waits if full buffer aka empty reached 0 from dequeue: BLOCKS
        down(&mutex);                       //thread waits for mutex lock once other thread finishes

        if (buffer.length != SIZE_OF_BUFFER) {                //if not full
            //insert next available node, copy each char bite into write->data char array
            is_copy = copy_from_user(buffer.write->data, data, DATA_LENGTH);

            if (is_copy != 0) {      //return value will be 0 if copy successful
                printk("\nBuffer_user.c Error copying the data into buffer.\n");
                return -1;
            }

            printk("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
            printk("Enqueue length pre-update: %d\n", buffer.length);
            printk("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

            buffer.write = buffer.write->next;             //advance the new cursor
            buffer.length++;                               //Increment the buffer length when you insert.

            up(&mutex);                       //thread releases mutex lock once it is finishes
            up(&fill_count);                  //thread increments the fill count

            return 0;
        }
    }
    //under any other circumstance error
    printk("\nBuffer_user.c Error enqueuing into buffer.\n");
    return -1;
}

SYSCALL_DEFINE1(dequeue_buffer_sem_421, char*, data) {
    int is_copy;                            //check return of copy
    //check if buffer is initialized
    if (buffer.read != NULL) {
        down(&fill_count);                  //thread waits if empty buffer aka fill reached 0 from enqueue: BLOCKS
        down(&mutex);                       //thread waits for mutex lock once other thread finishes

        if (buffer.length > 0) {                                //if not empty
            //read next available node, copy each char into pointer data char array
            is_copy = copy_to_user(data, buffer.read->next->data, DATA_LENGTH);

            if (is_copy != 0) {      //return value will be 0 if copy successful
                printk("\nBuffer_user.c Error copying the data into buffer.\n");
                return -1;
            }
            buffer.read = buffer.read->next;             //advance the cursor
            buffer.length--; //Increment the buffer length when you insert.

            printk("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
            printk("Dequeue length post-update:: %d\n", buffer.length);
            printk("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

            up(&mutex);                        //thread releases mutex lock once it is finishes
            up(&empty_count);                  //thread increments the empty count
            return 0;
        }
    }
    //under any other circumstance error
    printk("\nBuffer_user.c Error dequeuing buffer.\n");
    return -1;
}

SYSCALL_DEFINE0(delete_buffer_sem_421){
    //DECLARATIONS FOR THE KERNEL UGHHHH
    bb_node_421_t *temp; //so we can delete the malloc objects
    bb_node_421_t *end;
    // is Buffer initialized?
    if (buffer.read != NULL) {
        end = buffer.read;    //to keep track of where to stop but its circular
        // so just set current read node as the "end"
        buffer.read = buffer.read->next;   //start deleting at the "start" of linked list

        while (buffer.read!= end){        //until I reach the last node
            temp = buffer.read;
            buffer.read = buffer.read->next;    //go to next to be deleted
            kfree(temp);                             //delete current
        }
        //reset newBuff
        buffer.read = NULL;
        buffer.write = NULL;
        buffer.length = 0;

        //no need to free the semaphores
        kfree(end);                                  //delete last node
        return 0;
    }
    printk("\nBuffer_user.c Error deleting buffer.\n");
    return -1;
}




