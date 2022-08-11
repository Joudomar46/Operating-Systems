Part 3

User space:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
To compile:
Use Makefile

Command: 
make buff
./buff
valgrind ./buff
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Testing:
producer_enqueue() - 
Loops for 1000 times and has a random number generator to sleep the thread from 1 to 10 milli seconds.
Inserts a temp char array with numbers from 0-9 and starts back to 0 once 9 is reached. Also includes NULL value.
Calls enqueue function and keeps track of return values to check all 1000 times ran successfully. 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
consumer_dequeue() - 
Loops for 1000 times and has a random number generator to sleep the thread from 1 to 10 milli seconds.
Initialize a data char array to pass by reference to dequeue_buffer
Calls enqueue function and keeps track of return values to check all 1000 times ran successfully. 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
main()-
Create two threads one for producer and consumer.
~~~~~~~~~~~~~~~~~
Start with the main test asked in the documentation. 
Pass threads to the thread functions above to run a 1000 times. 
~~~~~~~~~~~~~~~~~
Try to initialize an initialized buffer already. 
~~~~~~~~~~~~~~~~~
Delete the buffer. 
~~~~~~~~~~~~~~~~~
Test enqueue and dequeue a non initialized buffer. 
~~~~~~~~~~~~~~~~~
Test delete noninitialized buffer. 
~~~~~~~~~~~~~~~~~
Test Initialize and delete an empty buffer. 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
buffer_user_sem.c implementation:

Create static buffer of type bb_buffer_421 and three sem_t semaphores
Mutex acts as a lock for the threads to allow working without interruption. 
Fill and empty count will allow the threads to wait for another thread to enqueue or dequeue.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
init_buffer_421(void)- 
Check if buffer initialized due to the buffer being static buffer.read is null returned the buffer is not initialized, other wise fail. 
If at any point function does not run to return 0, fail.

Temporary cursor to organize nodes.

Initialize the semaphores
Mutex will start at 1 
Fill_count starts at 0 and counts up as the buffer is filling.
empty_count starts at 20 and decreases as the buffer is enqueued. 

Memory for newBuff and set starting data.
For loop for the full size of buffer (20).
	Memory for newNode and set starting data. 
	2 cases, first node 
		or not first node. 

Use read and write pointer of newBuff
	read will be our global head aka tail points at head so read->next points at first element.
	write is our “cursor” for the next empty node 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
enqueue_buffer_421(char * data)-
Check if buffer initialized due to the buffer being static if not null is returned the buffer is initialized, 
other wise fail. If at any point function does not run to return 0, fail.

Threading checks!!!!!
Is the buffer full aka has empty count reached 0 from the original value of 20? 
Does the thread have the mutex key…if the mutex is equal to 0 it is in use by another thread. 

Check to see if we reached the max length of buffer, 

Loop through given char * data and copy each position into buffer.write->data, 
remember buffer.write starts at the “head” of the buffer. Advance the cursor and length.

This reaches the end of the critical section and the mutex can be released. Sem_post increments the
Value to 1. The fill_count semaphore also increments by one.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
dequeue_buffer_421(char * data)-
Check if buffer initialized due to the buffer being static if not null is returned the buffer is initialized, 
other wise fail. If at any point function does not run to return 0, fail.

Threading checks!!!!!
Is the buffer empty aka has fill count is greater than 0 from the sem_post of enqueue? 
Does the thread have the mutex key…if the mutex is equal to 0 it is in use by another thread. 

Check to see if buffer is not empty by length of buffer > 0, 

Loop through and copy each position from buffer.read->next->data, into passed reference char * data.
remember buffer.read starts at the “tail” of the buffer which is why the first data point is 
Buffer.read->next to be at head of circular buffer. Advance the cursor and length.

This reaches the end of the critical section and the mutex can be released. Sem_post increments the
Value to 1. The empty_count semaphore also increments by one.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
delete_buffer_421(void)-
Check if buffer initialized due to the buffer being static if not null returned the buffer is initialized, other wise fail. 
If at any point function does not run to return 0, fail.

Temporary cursor to organize nodes.
Temporary end to point at last element.
Deleting at “head” of buffer, set read->next to read
While loop to reach the last node in buffer 
	Typical free strategy
After loop ends 
	Reset newBuff values and set newBuff = 0 to reinitialize
	Delete the semaphores using sem_destroy().
	delete last node.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Kernel space:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
To compile:

Preparations prior to anything.
	⁃	Build a clean custom kernel and customizing kernel version to project 3 
	⁃	make -j<n> bindeb-pkg
	⁃	Append our system call function signature to include/linux/syscalls.h 
	⁃	asmlinkage long sys_init_buffer_sem_421(void);
	⁃	asmlinkage long sys_enqueue_buffer_sem_421(char * data);
	⁃	asmlinkage long sys_dequeue_buffer_sem_421(char * data));
	⁃	asmlinkage long sys_delete_buffer_sem_421(void);

	⁃	Add an entry for our syscall in arch/x86/entry/syscalls/syscall_64.tbl
	⁃	446     common  inti_buffer_sem_421 			sys_init_buffer_sem_421
	⁃	447     common  enqueue_buffer_sem_421       	sys_enqueue_buffer_sem_421
	⁃	448     common  dequeue_buffer_sem_421       	sys_dequeue_buffer_sem_421
	⁃	449     common  delete_buffer_sem_421       		sys_delete_buffer_sem_421

	⁃	Include our Makefile in the kernel Makefile. 
	⁃	obj-y := buffer_sem.o

	⁃	In kernel “Project 3” Makefile add prodcon_sem/ 

	⁃	rebuild kernel
	⁃	reboot kernel: sudo reboot
	⁃	Installing custom kernel 
	⁃	sudo dpkg -i
~~~~~~~~~~~~~~~~~
Makefile: 
Use Makefile

Command: 
make kernelTest
./test

See what was output in kernel
sudo dmesg
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Testing:
Extremely similar to the test_sem_user.c implementation except for the few differences:
Define the new System calls and create a function to call them which returns what they return.
Example:
#define __NR_init_buffer_sem_446
long init_buffer_sem_421_syscall(void){
	return syscall(__NR_init_buffer_sem_421);}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
buffer_sem.c implementation:
Extremely similar to the buffer_user.c implementation except for the few differences:

Must initialize at the top of each function and can not assign a value at the same time.

Function definitions are correlated to System calls so declaration is like so
SYSCALL_DEFINE0(init_buffer_sem_421)
SYSCALL_DEFINE1(enqueue_buffer_sem_421,char*, data)
SYSCALL_DEFINE1(dequeue_buffer_sem_421,char*, data)
SYSCALL_DEFINE0(delete_buffer_sem_421)

Using kmalloc instead of malloc for memory 
newBuff = kmalloc(sizeof(struct ring_buffer_421), GFP_KERNEL);

Using printk instead of printf for memory 
printk(“\nBuffer.c Error while initializing buffer.\n");

Using kfree instead of free for memory deletion
kfree(newBuff);  

Copy data safely using copy_from_usr and copy_to_usr

Fundamental logic stays consistent from the user to the kernel space.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
