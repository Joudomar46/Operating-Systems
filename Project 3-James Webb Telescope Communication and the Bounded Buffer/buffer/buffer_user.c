#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>


//static global variable
static ring_buffer_421_t *newBuff;

long init_buffer_421(void){
    //newBuff = 0 no buffer initialized
    if(!newBuff) {
        node_421_t *cursor = NULL; //temporary pointer head

        //create a buffer of type struct ring_buffer_421
        newBuff = malloc(sizeof(struct ring_buffer_421));
        newBuff->length = 0;    //will fill this in as we write data
        newBuff->read = NULL;
        newBuff->write = NULL;

        //create 20 Nodes for buffer
        for (int i = 0; i < SIZE_OF_BUFFER; i++) {
            node_421_t *newNode = malloc(sizeof(struct node_421));
            newNode->data = 0;      //fill in temp with 0
            newNode->next = NULL;   //for now
            //two cases
            //is it the first node created
            if (cursor == NULL) {
                newNode->next = newNode;    //point to itself
                cursor = newNode;
            } else {
                newNode->next = cursor->next;       //the next of new item will point to front of queue
                cursor->next = newNode;             //the current pointer to front will point to new item
                cursor = cursor->next;              //advance the new cursor
            }
        }
        //after filling the buffer make:
        newBuff->read = cursor;                         //read will act as global tail pointing to head
        if(newBuff->read->next != NULL) {               //write cursor points at the first element
            newBuff->write = newBuff->read->next;      //write will act as current cursor for information
        }

        return 0;
    }
    //under any other circumstance error
    fprintf(stderr, "\nBuffer_user.c Error while initializing buffer.\n");
    return -1;

}

long insert_buffer_421(int i) {
    //is Buffer initialized?
    if (newBuff) {
        if (newBuff->length != SIZE_OF_BUFFER) {                //if not full
            //insert next available node.
            newBuff->write->data = i;
            newBuff->write = newBuff->write->next;             //advance the new cursor
            newBuff->length++; //Increment the buffer length when you insert.
            return 0;
        }
    }
    //under any other circumstance error
    fprintf(stderr, "\nBuffer_user.c Error inserting into buffer.\n");
    return -1;
}


long print_buffer_421(void){
    //check if buffer is initialized
    if(newBuff) {
        node_421_t *temp = newBuff->read->next;             //we start at front item in head
        //we continue visiting nodes until we reach the tail
        while (newBuff->read != NULL && temp != newBuff->read) {
            printf("(%d) ", temp->data);
            temp = temp->next;              //advance cursor
        }
        //at the end we visit the cursor
        //this also covers the case that there is only one item
        if (newBuff->read != NULL)
            printf("(%d) ", newBuff->read->data);

        return 0;
    }
    //under any other circumstance error
    fprintf(stderr, "\nBuffer_user.c Error printing buffer.\n");
    return -1;
}

long delete_buffer_421(void){
    //is Buffer initialized?
    if (newBuff) {
        node_421_t *temp;; //so we can delete the malloc objects
        node_421_t *end = newBuff->read;    //to keep track of where to stop
        newBuff->read = newBuff->read->next;   //start deleting at the start of linked list


        while (newBuff->read!= end)        //until I reach the last node
        {
            temp = newBuff->read;
            newBuff->read = newBuff->read->next;    //go to next to be deleted
            free(temp);                             //delete current
        }
        //reset newBuff
        newBuff->read = NULL;
        newBuff->write = NULL;
        newBuff->length = 0;

        free(end);                                  //delete last node
        free(newBuff);                              //delete the buffer container

        //reset newBuff = 0 in order to initialize again after deletion
        newBuff = 0;
        return 0;
    }
    fprintf(stderr, "\nBuffer_user.c Error deleting buffer.\n");
    return -1;

}

