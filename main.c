#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_TOKENS 64
#define DELIM " \t\r\n" // tab, carriage return, newline

// function prototypes
char *read_line(void);
int parse_line(char *line, char **args);
int execute_command(char **args);

int main(void) {
    char *line;
    char *args[MAX_TOKENS];
    int should_run = 1;

    while (should_run) {
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

// for commands like ls, cat, grep -> they are separate linux programs, we will be running the linux 

