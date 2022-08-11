//
// Created by Joud Omar.
//

#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include "utils.h"


//FUNCTION PROTOTYPES
void shell_loop();  //endless loop for shell prompt
char *read_command();   //read the command from user
char **parse_command(char *user_command);
void flush_str_input(char *cp);
int exitArg(char *arg);
void exit_build_in(char *program, char **arg_list);
int excecute_command(char *program, char **arg_list);
char* concat(const char *s1, const char *s2); //helper fun


//utils.h returns
//count_spaces(const char *str); returns num of spaces in a line.
//flush_input(FILE *fp);         read until we hit EOF or /n
//char *unescape(const char *str, FILE *errf);
//int first_unquoted_space(const char *str);

//endless loop of the shell that displays it
//waits for user command
//parse the command
//after command is executed, displays again
//exits

//helper function from
//https://stackoverflow.com/questions/8465006/how-do-i-concatenate-two-strings-in-c
//to concat in proc
char* concat(const char *s1, const char *s2)
{
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1); // +1 to copy the null-terminator
    return result;
}
//shell shall present the user with a prompt
//at which they can enter commands.
//Upon the completion of a command,
//it shall prompt the user for a new command.
void shell_loop() {
    char *user_command;
    char **args;
    int status = 1;
    do {
        printf("UserName@root > ");
        fflush(stdout);

        user_command = read_command();  //read the command

        //loop until we get a real input from the user
        if(user_command[0] == '\n'){
            shell_loop();
        }
        //clean the command from the \n at the end of the string
        flush_str_input(user_command);

        args = parse_command(user_command);



        //check for exit command
        if(strcmp(args[0],"exit")==0) {
            exit_build_in(args[0],args);
        }//proc only takes one argument
        else //check for proc command
            if(strcmp(args[0], "proc") == 0){

            char *temp = "/proc/";
            char *filename = concat(temp,args[1]);

                FILE *fp;

                fp = fopen(filename, "r"); // read mode

                if (fp == NULL) {
                    perror("Error while opening the file.\n");
                    exit(1);
                }
                char ch;
                while ((ch = fgetc(fp)) != EOF)
                    printf("%c", ch);

                fclose(fp);
                printf("\n");
            }
        else { //execute the command using fork and exec
            status = excecute_command(args[0],args);
        }

    } while (status);

}

//remove enter from end of string used for user input formatting
void flush_str_input(char *cp){
    char *temp;
    if((temp = strchr(cp, '\n')) != NULL)
        *temp = '\0';
}

int exitArg(char *arg){
    //char str = arg;
    int x = atoi(arg);
    return x;
}
//built in exit feature
// 0 arguments = exit 0
// 1 argument = exit (#)
// 1 argument with non number = try again
// 2 arguments try to process first argument
void exit_build_in(char *program, char **arg_list){
    //if 0 arguments
    if(arg_list[1] ==NULL){
        free(program);
        program = NULL;
        free(arg_list);
        arg_list = NULL;
        exit(0);
    }
    //use helper function to convert char to int
    int code = exitArg(arg_list[1]);
    if((code) >= 0){
        free(program);
        program = NULL;
        free(arg_list);
        arg_list = NULL;
        exit(code);
    }
}
//getline (&string, &size, stdin);
//unspecified size string getline does the work
//returns the string and includes \n at the end
char *read_command(){
    char *string = NULL;
    size_t size = 0;
    //error checking for issues with user input

    if(getline(&string,&size,stdin) == -1){
        fprintf(stderr, "read_command error, unable to read the user command.");
        exit(1);
    } else { //else it started correctly and we can start the loop
        return string;
    }

}

//time to parse, aka break up line into arguments
char **parse_command(char *user_command) {
    int position = 0;
    int space = 0;
    int bufSize = 64;
    char token[64];

    char **tokens = malloc(64 * sizeof(char *));


    //case 1: only one command no parameters
    if (count_spaces(user_command) == 0) {
        tokens[position] = user_command;
        tokens[position + 1] = NULL;
        return tokens;
    }
    //case 2 command with parameters
    do {
        space = first_unquoted_space(user_command);
        strncpy(token,user_command,space);           //find space position, and copy it to token array
        token[space] = '\0';
        if(!isspace(user_command[0])){            //removing the extra spaces between arguments
            tokens[position]  = unescape(token,stderr);
            position++;
        }
        user_command += space + 1;

        //error checking and resizing to make tokens array bigger
        if(position >= bufSize){
            bufSize += 64;
            tokens = realloc(tokens,64*sizeof(char*));
            if(!tokens){
                fprintf(stderr, "Allocation Error\n");
                exit(1);
            }
        }
    } while(first_unquoted_space(user_command) != -1);

    //checking for unnecessary spaces
    if(isspace(user_command[0])){
        tokens[position] = NULL;
    }else {
        tokens[position++] = unescape(user_command, stderr); //check last token
        tokens[position] = NULL;
    }

    //checking for errors
    int i = 0;
    int e = 0;
    while (i<=position) {
        if (tokens[i]==NULL){
            e++;
        }
        //printf("Token %d: %s\n", i, tokens[i]);
        i++;
    }
    if(e > 1){
        shell_loop();
    }

    return tokens;
}

int excecute_command(char *program, char **arg_list){
    pid_t pid,wpid;
    int status;
//    printf("Program: %s\n",program);
//    printf("arg list: %s\n",arg_list[1]);
    pid = fork();
    if (pid == 0) {
        //child process
        //printf("In child Process\n");
        //int execvp(const char *file, char *const argv[]);
        if (execvp(program,arg_list)){
            fprintf(stderr, "Command not found\n");
            exit(1);
        }
    }else if (pid < 0){
        //error forking
        fprintf(stderr, "Fork error in excecution pid < 0\n");
    }else //parent process > 1
    {
        do{
                //printf("waiting\n");
            wpid = waitpid(pid, &status, WUNTRACED);
        }while (!WIFEXITED(status) && !WIFSIGNALED(status));
        //printf("Parent?\n");
    }

    return 1;
}

//main, starting shell
//If executed with any arguments,
// the shell shall print an error message to stderr
// explaining that it does not accept any command
// line arguments and exit with an error return code (1).
int main(int argc, char *argv[]) {
    //error checking shell starting incorrectly exit code 1
    if(argc > 1){
        fprintf(stderr, "The command \"%s\" accepts no arguments.", argv[0]);
        exit(1);
    } else { //else it started correctly and we can start the loop
        fprintf(stdout, "Welcome to simple shell.\n");
        shell_loop();
    }
    return 0;
}
