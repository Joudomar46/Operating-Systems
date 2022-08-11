#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "task.h"
#include "list.h"
#include "schedulers.h"

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
               "./fcfs .schedule.txt.\n");
        printf("The file is read through and add() is called based on the specified scheduler algorithm: "
               "make fcfs.\nThis program takes in each task, formats the task from txt to create an object of "
               "type Task, link to a newNode,\nThe global head node variable will keep track of where the start "
               "of the list is.\nThen insert each node as they are created to a linked list.\nSchedule() is called from "
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

    printf("Creating linked list. Inserting the nodes in the order of FCFS.\n\n");
    //inserting the nodes in the order of First Come First Served
    //and creating the linked list
    if(head==NULL){
        head = newNode;
    }else{
        struct node* temp = NULL;
        temp = head;
        while(temp->next != NULL){//traverse
            temp = temp->next;
        }
        temp->next = newNode;
    }
    i++;
}


void schedule(){
    //traverse each node and call function run from CPU.c
    //since we ordered each task in FCFS in the add function the head node
    //head node will point to the first task to be ran
    printf("Now the linked list has all the tasks in the order of FCFS.\n");
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



