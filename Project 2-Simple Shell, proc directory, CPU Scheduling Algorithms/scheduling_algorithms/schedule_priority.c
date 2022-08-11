#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "task.h"
#include "list.h"
#include "schedulers.h"

//schedules tasks in order of the length of the tasks' priority 10 is most important 1 is not.
//highest priority to the lowest priority.

//1. A brief, well-formatted explanation of the algorithm demonstrating your understanding of
//the algorithm before it starts executing.
//2. Informative statements that briefly explain each step of the algorithm as it is taking place.

//Global head node;
struct node* head = NULL;
int i = 1;

void add(char *name, int priority, int burst){
    //printing statement once;
    if(i == 1) {
        printf("\nBefore execution: The algorithm starts in drive.o which takes a txt file in the command line:"
               "./priority .schedule.txt.\n");
        printf("The file is read through and add() is called based on the specified scheduler algorithm: "
               "make priority.\nThis program takes in each task, formats the task from txt to create an object of "
               "type Task, link to a newNode,\nThe global head node variable will keep track of where the start "
               "of the list is.\nThen insert each node as they are created to a linked list while comparing their "
               "priority values to place in order.\nSchedule() is called from "
               "drive.c, schedule implementation traverses the linked list and calls run from CPU.c.\n"
               "Clean up job is done to free all allocated memory at the end.\n\n");
    }
    //create a new task of type struct Task
    printf("Task number: %d\n",i);
    printf("Creating newTask to put information passed from driver.c\n");
    struct task* newTask = malloc(sizeof(struct task));

    newTask->name = name;
    newTask->tid = 0;
    newTask->priority = priority;
    newTask->burst = burst;

    printf("Creating newNode, linking it's task to newTask.\n");
    //create a new node of type struct node
    struct node* newNode = malloc(sizeof(struct node));
    newNode->task = newTask;
    newNode->next = NULL;

    printf("Creating linked list. Inserting the nodes in order of largest priority to smallest.\n\n");
    //inserting the nodes in the order
    //and creating the linked list
    //case for no node in list
    if(head==NULL){
        head = newNode;
    }else{
        struct node* temp = NULL;
        temp = head;
        //in the case of the head task being less than the newTask
        // 10 -> 20 want newNode to be new head 20->10
        if(temp->task->priority <= newNode->task->priority){
            newNode->next = head;
            head = newNode;
        }else {
            while (temp->next != NULL && temp->next->task->priority > newNode->task->priority) {//traverse
                temp = temp->next;
            }
            newNode->next = temp->next;
            temp->next = newNode;
        }
    }
    i++;
}


void schedule(){
    //traverse each node and call function run from CPU.c
    //Traverse from head to end since tasks are already in correct order.

    printf("Now the linked list has all the tasks in order we want.\n");
    printf("Begin traversal for each task to run in cpu.c.\n\n");
    struct node* temp = NULL;
    temp = head;

    while (temp != NULL) {
        run(temp->task,temp->task->burst);
        temp = temp->next;
    }

    temp = head; //so we can delete
    //delete the malloc objects task:name, task struct, and node struct
    //delete each step
    printf("\nFinally, deallocate memory using free().\n");
    struct node* temp2;

    while (head != NULL)
    {
        temp2 = head;
        head = head->next;
        free(temp2->task->name);
        free(temp2->task);
        free(temp2);
    }
}



