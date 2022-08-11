Part 1 
Sorry it’s long, they released the rubric after I already wrote all this, size is large and pics added. 
Please don’t deduct points for this :)!!!!!
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
returnValueMsg() - 
checks the return value of functions in buffer_user and expected value from the test preformed.
~~~~~~~~~~~~~~~~~
nonInitializedCaseTest() - 
Test for printing, inserting, and deleting when buffer is not initialized. All should fail. 
~~~~~~~~~~~~~~~~~
fullBuffer()- 
Initialized buffer.  Print buffer with zeros.
Test to initialize again, will fail. 
Insert 20 values into buffer.
Print buffer with values.
Test to insert after buffer is full, will fail. 
Print after failed insertion. 
Delete buffer. 
~~~~~~~~~~~~~~~~~
halfBuffer()- 
Initialized buffer. 
Print buffer with zeros.
Insert half (10) values into buffer.
Print buffer with values.
Delete buffer. 
This test is mainly to check for data leaks if buffer is not full.
~~~~~~~~~~~~~~~~~
oneElement()-
Initialized buffer. 
Print buffer with zeros.
Insert one value into buffer.
Print buffer with values.
Delete buffer. 
This test is mainly to check for data leaks if buffer has one element.
~~~~~~~~~~~~~~~~~
noElement()-
Initialized buffer. 
Print buffer with zeros.
Delete buffer. 
This test is mainly to check for data leaks if buffer has no elements.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
buffer_user.c implementation:
Create static ring_buffer newBuff pointer
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
init_buffer_421(void)- 
Check if buffer initialized due to the buffer being static if 0 returned the buffer is not initialized, other wise fail. 
If at any point function does not run to return 0, fail.

Temporary cursor to organize nodes.

Memory for newBuff and set starting data.
For loop for the full size of buffer (20).
	Memory for newNode and set starting data. 
	2 cases, first node 
		or not first node. 

Use read and write pointer of newBuff
	read will be our global head aka tail points at head so read->next points at first element.
	write is our “cursor” for the next empty node 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
insert_buffer_421(int i)-
Check if buffer initialized due to the buffer being static if 1 returned the buffer is initialized, other wise fail. 
If at any point function does not run to return 0, fail.

Check to see if we reached the max length of buffer, 
Cursor should be at the start initially then advances after inserting the required data.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
print_buffer_421(void)-
Check if buffer initialized due to the buffer being static if 1 returned the buffer is initialized, other wise fail. 
If at any point function does not run to return 0, fail.

Temporary cursor to organize nodes.
	while loop to iterate over all nodes
	case for one node only // not really used since all nodes initialized 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
delete_buffer_421(void)-
Check if buffer initialized due to the buffer being static if 1 returned the buffer is initialized, other wise fail. 
If at any point function does not run to return 0, fail.

Temporary cursor to organize nodes.
Temporary end to point at last element.
Deleting at “head” of buffer, set read->next to read
While loop to reach the last node in buffer 
	Typical free strategy
After loop ends 
	Reset newBuff values and set newBuff = 0 to reinitialize
	delete last node and newBuff.
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
	⁃	asmlinkage long sys_init_buffer_421(void);
	⁃	asmlinkage long sys_insert_buffer_421(int i);
	⁃	asmlinkage long sys_print_buffer_421(void);
	⁃	asmlinkage long sys_delete_buffer_421(void);

	⁃	Add an entry for our syscall in arch/x86/entry/syscalls/syscall_64.tbl
	⁃	442     common  inti_buffer_421         	sys_init_buffer_421
	⁃	443     common  insert_buffer_421     	sys_insert_buffer_421
	⁃	444     common  print_buffer_421        	sys_print_buffer_421
	⁃	445     common  delete_buffer_421       	sys_delete_buffer_421

	⁃	Include our Makefile in the kernel Makefile. 
	⁃	obj-y := buffer.o

	⁃	In kernel “Project 3” Makefile add buffer/ 
	
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
Extremely similar to the test_user.c implementation except for the few differences:
Define the new System calls and create a function to call them which returns what they return.

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

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
buffer.c implementation:
Extremely similar to the buffer_user.c implementation except for the few differences:

Must initialize at the top of each function and can not assign a value at the same time.

Function definitions are correlated to System calls so deceleration is like so
SYSCALL_DEFINE0(init_buffer_421)
SYSCALL_DEFINE1(insert_buffer_421, int, i)
SYSCALL_DEFINE0(print_buffer_421)
SYSCALL_DEFINE0(delete_buffer_421)

Using kmalloc instead of malloc for memory 
newBuff = kmalloc(sizeof(struct ring_buffer_421), GFP_KERNEL);

Using printk instead of printf for memory 
printk(“\nBuffer.c Error while initializing buffer.\n");

Using kfree instead of free for memory deletion
kfree(newBuff);  

Fundamental logic stays consistent from the user to the kernel space.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Thank you for coming to my TedTalk. 
