• Present the user with some sort of prompt at which the user may enter a command to execute.

- the main file calls the shells main loop in which the read_command prompts user to enter.
Their commands. 

• Parse out the program the user is attempting to call from its arguments and build an appropriate argument array which can be used to execute the program.

-the command goes into parse_command() where we check for one or more tokens. If more than one token
We find the first unquoted space to single out the argument. The argument is passed to unescape.
Then we can create the array of different tokens. First token is the argument, the rest are 
Paramaters. 

• Determine if the program specified is a built-in (proc, or exit) and handle those functions without creating a new process or attempting to execute another program (except as might be required for proc).

-after parsing, the argument array is then checked for exit command or proc command. This happens with a simple strcmp. If that is the case, the program enters the specific algorithm to exit or proc. 


• If the program specified is not built into the shell like “exit”, your shell must create a new process to execute the new program and pass in the correct arguments to one of the exec family of functions to execute the program with the arguments provided. Your shell then must wait for the newly created process to finish executing. Your shell must also handle the case in which a program cannot be executed properly and print out an appropriate error message to stderr.

-lastly if not exit or proc, execute_command() forks and execvp execute based on first element of argslist being the command and the rest is the parameters to act on. Following a series of if/else to determine the forking/exec process completed correctly. 

• Once the specified built-in or program has been executed (or failed executing), your shell should prompt the user for another command to run (unless the shell has exited from the exit built-in command).

The shell loop runs forever and will prompt the user to enter a command until exit. 

Sources of inspiration:
https://brennan.io/2015/01/16/write-a-shell-in-c/
https://danishpraka.sh/2018/01/15/write-a-shell.html
https://cs.brown.edu/courses/cs033/docs/proj/shell1.pdf
https://www.youtube.com/watch?v=z4LEuxMGGs8&ab_channel=FooSo
https://www.youtube.com/watch?v=spD11nnLsLE&ab_channel=IanShaw
https://www.youtube.com/watch?v=k6TTj4C0LF0&t=207s&ab_channel=wikimediaDE
