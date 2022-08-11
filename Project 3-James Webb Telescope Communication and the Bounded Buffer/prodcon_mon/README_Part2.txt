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
Test delete non-initialized buffer. 
~~~~~~~~~~~~~~~~~
Test Initialize and delete an empty buffer. 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
buffer_mon.c implementation:

Create static buffer of type bb_buffer_421 and one pthread_mutex and two pthread_cond
Mutex acts as a lock for the threads to allow working without interruption. 
Fill and empty count will allow the threads to wait for another thread to enqueue or dequeue.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
init_buffer_421(void)- 
Check if buffer initialized due to the buffer being static buffer.read is null returned the buffer is not initialized, other wise fail. 
If at any point function does not run to return 0, fail.

Temporary cursor to organize nodes.

Initialize the monitor

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
Does the thread have the mutex key…if the mutex is locked it is in use by another thread. 
While the buffer length is greater than or equal to 20, the condition must wait. 
The thread waits for the other thread to signal the empty condition and the while loop 
Ends since the length will be updated. 

Check to see if we reached the max length of buffer, 

Loop through given char * data and copy each position into buffer.write->data, 
remember buffer.write starts at the “head” of the buffer. Advance the cursor and length.

This reaches the end of the critical section and the signal for fill_count is given, mutex can be released. ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
dequeue_buffer_421(char * data)-
Check if buffer initialized due to the buffer being static if not null is returned the buffer is initialized, 
other wise fail. If at any point function does not run to return 0, fail.

Threading checks!!!!!
Does the thread have the mutex key…if the mutex is locked it is in use by another thread. 
While the buffer length is less than or equal to 0, the condition must wait. 
The thread waits for the other thread to signal the fill condition and the while loop 
Ends since the length will be updated. 

Check to see if buffer is not empty by length of buffer > 0, 

Loop through and copy each position from buffer.read->next->data, into passed reference char * data.
remember buffer.read starts at the “tail” of the buffer which is why the first data point is 
Buffer.read->next to be at head of circular buffer. Advance the cursor and length.

This reaches the end of the critical section and the signal for empty_count is given, mutex can be released. 
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
	Delete the monitor
	delete last node.
