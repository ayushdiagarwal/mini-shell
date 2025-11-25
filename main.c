#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>

#define MAX_TOKENS 64
#define DELIM " \t\r\n" // tab, carriage return, newline

// function prototypes
char *read_line(void);
int parse_line(char *line, char **args);
int execute_command(char **args);
int handle_builtin(char **args);

int main(void) {
    char *line;
    char *args[MAX_TOKENS];

    while (1) {
        // print prompt
        printf("pintu> ");
        fflush(stdout);

        // read input line 
        line = read_line();
        if (line == NULL) {
            // exit shell
            printf("Exiting shell ... \n");
            break;
        }

        // parse line into arguments
        int argc = parse_line(line, args);
        if (argc == 0) {
            // empty line 
            free(line);
            continue;
        }

        // exit command
        if (strcmp(args[0], "exit") == 0) {
            free(line);
            break;
        }

        // check if it's built-in command
        if (handle_builtin(args)) {
            // built-in handled
            free(line);
            continue;
        }

        // execute external command?
        execute_command(args);

        // free allocated memory
        free(line);

    }

    return 0;
}

// read line from stdin using getline 
// returns heap-allocated string

char *read_line(void) {

    char *line = NULL;
    size_t bufsize = 0;
    ssize_t nread = getline(&line, &bufsize, stdin);

    if (nread == -1) {
        // error or EOF
        free(line);
        return NULL;
    }

    return line;
}

// splits the line into tokens and fills args[]
// returns number of tokens 

int parse_line(char *line, char **args) {

    int position = 0;
    // strtok is used to split the line into tokens
    char *token = strtok(line, DELIM);

    while (token != NULL && position < MAX_TOKENS -1) {
        args[position++] = token;
        // strtok returns the next token or NULL if no more tokens are found
        token = strtok(NULL, DELIM);
    }

    args[position] = NULL;
    return position;
}

// for commands like ls, cat, grep -> they are separate linux programs, we will be running the linux 

// handle built-in commands 
// returns 1 if command is built-in and was handled, 0 otherwise

int handle_builtin(char **args) {
    
    // cd
    if (strcmp(args[0], "cd") == 0) {
        // no argument -> go to home directory
        char *target = args[1];

        if (target == NULL) {
            target = getenv("HOME");
            if (target == NULL) {
                fprintf(stderr, "cd: HOME not set\n");
                return 1;
            }
        }


        if (chdir(target) != 0) {
            perror("cd");
        }

        return 1;
    }

    // pwd 
    if (strcmp(args[0], "pwd") == 0) {
        char cwd[PATH_MAX];

        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("pwd");
        }

        return 1;
    }

    // echo
    if (strcmp(args[0], "echo") == 0) {
        int i = 1;
        while (args[i] != NULL) {
            printf("%s", args[i]);
            if (args[i + 1] != NULL) {
                printf(" ");
            }
            i++;
        }
        printf("\n");
        return 1;
    }    

    // clear
    if (strcmp(args[0], "clear") == 0) {
        // ANSI escape code to clear screen and move cursor to top-left
        printf("\033[H\033[J");
        return 1;
    }

    // help
    if (strcmp(args[0], "help") == 0) {
        printf("pintu shell  - built-in commands:\n");
        printf(" cd <dir>    - change directory\n");
        printf(" pwd         - print current directory\n");
        printf(" echo <args> - prints the arguments\n");
        printf(" clear       - clear the screen\n");
        printf(" help        - show this help message\n");
        printf(" exit        - exit the shell\n");
        printf(" about       - show shell information\n");

        return 1;
    }

    if (strcmp(args[0], "about") == 0) {
        printf("pintu shell - a simple shell written in C\n");
        printf("Author: Ayush Agarwal\n");
        printf("Features: Built-in commands, External commands, Error handling\n");
        return 1;
    }

    // not a built-in 
    return 0;
}

// forks and executes the command in arg[0] with args[]

int execute_command(char **args) {
    pid_t pid = fork();

    if (pid < 0){ // fork failed
        perror("fork failed");
        return -1;
    } else if (pid == 0) {
        // child process
        if (execvp(args[0], args) == -1) {
            perror("pintu: exec failed");
        }
        exit(EXIT_FAILURE);
    } else {
        // parent process - wait for child to finish
        int status;
        do {
            // waitpid is used to wait for the child process to finish
            if (waitpid(pid, &status, 0) == -1) {
                perror("waitpid failed");
                return -1;
            }
            // WIFEXITED is used to check if the child process exited normally
            // WIFSIGNALED is used to check if the child process was terminated by a signal
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));

        return 0;
    }
}