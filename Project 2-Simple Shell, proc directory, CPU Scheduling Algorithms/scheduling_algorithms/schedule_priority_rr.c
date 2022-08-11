#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "task.h"
#include "list.h"
#include "schedulers.h"

//schedules tasks in order of priority and uses
//round-robin scheduling for tasks with equal priority.

//1. A brief, well-formatted explanation of the algorithm demonstrating your understanding of
//the algorithm before it starts executing.
//2. Informative statements that briefly explain each step of the algorithm as it is taking place.

//Global head node;
struct node* head = NULL;
int i = 1;
int list_size = 1;

void add(char *name, int priority, int burst){
    //printing statement once;
    if(i == 1) {
        printf("\nBefore execution: The algorithm starts in drive.o which takes a txt file in the command line:"
               "./rr .schedule.txt.\n");
        printf("The file is read through and add() is called based on the specified scheduler algorithm: "
               "make rr.\nThis program takes in each task, formats the task from txt to create an object of "
               "type Task, link to a newNode,\nThe global head node variable will keep track of where the start "
               "of the list is.\nThen insert each node as they are created to a linked list in priority order."
               "\nSchedule() is called from drive.c, schedule implementation traverses the linked list and "
               "calls run from CPU.c.\nComparing burst to quantum each time until all tasks are less than quantum.\n"
               "This only happens to tasts with repeated priority\n. Run is only called on "
               "tasts with busrts greater than quantum.\n"
               "Clean up job is done to free all allocated memory at the end.\n\n");
        i = 0;
    }
    //create a new task of type struct Task
    printf("Task number: %d\n",list_size);
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
    list_size++;
}


void schedule(){
    //traverse each node and call function run from CPU.c
    //head node  will point to the first task to be ran
    printf("Now the linked list has all the tasks in the order of Priority.\n");
    printf("Begin traversal for each task to run in cpu.c.\n\n");

    struct node* temp = NULL;
    temp = head;

    printf("Starting first round:\n");
    //first time showing
    while (temp != NULL) {
        run(temp->task,temp->task->burst);
        temp = temp->next;
    }
    printf("\nStarting next round:\n");
    temp = head; //reset temp to the start
    int counter = 1;
    int repeatingP; //keep track of prior Priority value
    //running this multiple times until the end of list size
    while(counter < list_size) {
        counter = 1;
        while (temp != NULL) {
            //checking to see if the next task priority is repeating
            if(temp->next != NULL && temp->task->priority == temp->next->task->priority){

                if (temp->task->burst >= QUANTUM) {
                    temp->task->burst = temp->task->burst - 10;
                    run(temp->task, temp->task->burst);
                } else {
                    printf("Task removed and will not run.\n");
                    counter++;
                }
                repeatingP = temp->task->priority;
            }else if (repeatingP == temp->task->priority){
                //if prior task priority=current priority run RR
                if (temp->task->burst >= QUANTUM) {
                    temp->task->burst = temp->task->burst - 10;
                    run(temp->task, temp->task->burst);
                } else {
                    printf("Task removed and will not run.\n");
                    counter++;
                }
            }else{
                //no repeating prority
                run(temp->task, temp->task->burst);
                counter++;
            }
            temp = temp->next;
        }
        if(counter == list_size){
            printf("\n-------We are done-------\n");
        }else
            printf("\nStarting next round:\n");
        temp = head;    //reset the temp to start at the start of list
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



