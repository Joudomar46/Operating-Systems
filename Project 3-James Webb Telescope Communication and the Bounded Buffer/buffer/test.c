#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include "buffer.h"

#define __NR_init_buffer_421 442
long init_buffer_421_syscall(void) {
    return syscall(__NR_init_buffer_421);
}
#define __NR_insert_buffer_421 443
long insert_buffer_421_syscall(int i) {
    return syscall(__NR_insert_buffer_421, i);
}
#define __NR_print_buffer_421 444
long print_buffer_421_syscall(void) {
    return syscall(__NR_print_buffer_421);
}
#define __NR_delete_buffer_421 445
long delete_buffer_421_syscall(void) {
    return syscall(__NR_delete_buffer_421);
}

void returnValueMsg(long returned, int correct){
    //what was the return value?
    //is it equal to what we expected
    if(returned == correct){
        printf("\nThe return value (%ld) is what we expected in this test.\n",returned);
    }else{
        printf("\nThe return value (%ld) is NOT what we expected in this test.\n",returned);
    }
}

void nonInitializedCaseTest(){
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("NON INITIALIZED BUFFER CASES->\n");

    printf("Attempt to print:");
    returnValueMsg(print_buffer_421_syscall(), -1);

    printf("~~~~~~~~~~~~~~~\n");
    printf("Attempt to insert:");
    returnValueMsg(insert_buffer_421_syscall(200), -1);

    printf("~~~~~~~~~~~~~~~\n");
    printf("Attempt to delete:");
    returnValueMsg(delete_buffer_421_syscall(), -1);

    printf("END OF NON INITIALIZED BUFFER CASES!");
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

void fullBuffer(){
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("FULL BUFFER CASES->\n");

    printf("Start with initializing a buffer:");
    returnValueMsg(init_buffer_421_syscall(), 0);
    printf("PRINTING BUFFER:\n");
    returnValueMsg(print_buffer_421_syscall(), 0);

    printf("~~~~~~~~~~~~~~~\n");
    printf("Attempt to create another buffer:");
    returnValueMsg(init_buffer_421_syscall(), -1);

    printf("~~~~~~~~~~~~~~~\n");
    printf("Full insert into buffer:\n");
    for(int i =0; i<20;i++) {
        insert_buffer_421_syscall(i);
    }
    printf("PRINTING BUFFER:\n");
    returnValueMsg(print_buffer_421_syscall(), 0);

    printf("~~~~~~~~~~~~~~~\n");
    printf("Insert another when full:");
    returnValueMsg(insert_buffer_421_syscall(200), -1);
    printf("PRINTING BUFFER:\n");
    returnValueMsg(print_buffer_421_syscall(), 0);

    printf("~~~~~~~~~~~~~~~\n");
    printf("Delete buffer case full:");
    returnValueMsg(delete_buffer_421_syscall(), 0);

    printf("END OF FULL BUFFER CASES!");
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

void halfBuffer(){
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("HALF BUFFER CASES->\n");

    printf("Start with initializing a buffer:");
    returnValueMsg(init_buffer_421_syscall(), 0);
    printf("PRINTING BUFFER:\n");
    returnValueMsg(print_buffer_421_syscall(), 0);

    printf("~~~~~~~~~~~~~~~\n");
    printf("Half full insert into buffer:\n");
    for(int i =0; i<10;i++) {
        insert_buffer_421_syscall(i);
    }
    printf("PRINTING BUFFER:\n");
    returnValueMsg(print_buffer_421_syscall(), 0);

    printf("~~~~~~~~~~~~~~~\n");
    printf("Delete buffer case half full:");
    returnValueMsg(delete_buffer_421_syscall(), 0);

    printf("END OF HALF BUFFER CASES!");
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

void oneElement(){
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("ONE ELEMENT BUFFER CASES->\n");

    printf("Start with initializing a buffer:");
    returnValueMsg(init_buffer_421_syscall(), 0);
    printf("PRINTING BUFFER:\n");
    returnValueMsg(print_buffer_421_syscall(), 0);

    printf("~~~~~~~~~~~~~~~\n");
    printf("Insert one element only:");
    returnValueMsg(insert_buffer_421_syscall(200), 0);
    printf("PRINTING BUFFER:\n");
    returnValueMsg(print_buffer_421_syscall(), 0);

    printf("~~~~~~~~~~~~~~~\n");
    printf("Delete buffer case one element only:");
    returnValueMsg(delete_buffer_421_syscall(), 0);

    printf("END ONE ELEMENT BUFFER CASES!");
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

void noElement(){
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("NO ELEMENT BUFFER CASES->\n");

    printf("Start with initializing a buffer:");
    returnValueMsg(init_buffer_421_syscall(), 0);
    printf("PRINTING BUFFER:\n");
    returnValueMsg(print_buffer_421_syscall(), 0);

    printf("~~~~~~~~~~~~~~~\n");
    printf("Delete buffer case no elements:");
    returnValueMsg(delete_buffer_421_syscall(), 0);

    printf("END NO ELEMENT BUFFER CASES!");
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}


int main() {
    //Refer to README.
    nonInitializedCaseTest();
    fullBuffer();
    halfBuffer();
    oneElement();
    noElement();
    return 0;
}
