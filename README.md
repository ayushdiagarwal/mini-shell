# **MiniShell**

A lightweight command-line shell written in C that demonstrates core Operating System concepts including process creation, system calls, I/O handling, and command execution.

---

## Overview

**MiniShell** is a simplified Unix-like shell built as an academic Operating Systems project.
The shell supports:

* Executing Linux external commands (`ls`, `cat`, `grep`, etc.)
* Built-in commands to control the shell environment (`cd`, `echo`, `help`, etc.)
* Interactive input loop similar to Bash

This project focuses on how a shell parses commands, creates processes, interacts with the OS through system calls, and handles basic user interaction.

---

## Objectives

This project demonstrates key OS concepts:

* **System Calls:** `fork()`, `execvp()`, `waitpid()`, `chdir()`, `getcwd()`, etc.
* **Process Management:** launching child processes and synchronizing using `wait()`
* **Command Parsing:** tokenizing input using delimiters
* **Built-in command execution vs. external binary execution**

---

## Features

| Feature                              | Type     | Status    |           |
| ------------------------------------ | -------- | --------- | --------- |
| Command prompt loop                  | Core     | ✔️        |           |
| Execute external Linux commands      | Core     | ✔️        |           |
| `exit` command                       | Built-in | ✔️        |           |
| `cd <directory>`                     | Built-in | ✔️        |           |
| `pwd`                                | Built-in | ✔️        |           |
| `echo`                               | Built-in | ✔️        |           |
| `clear`                              | Built-in | ✔️        |           |
| `help`                               | Built-in | ✔️        |           |
| `about`                              | Built-in | ✔️        |           |
| Error handling for invalid commands  | Core     | ✔️        |           |
---

## Compilation & Execution

### **Prerequisites**

* GCC compiler
* Linux environment (recommended)

### **Compile**

```bash
gcc main.c -o minishell
```

### **Run**

```bash
./minishell
```

---

## Sample Usage

```
minishell> pwd
/home/ayush

minishell> cd Documents

minishell> echo Hello from minishell shell
Hello from minishell shell

minishell> ls -l
# Output from real Linux 'ls'

minishell> help
# List available built-in commands

minishell> exit
```

---

## 🧠 System Calls Used

| System Call | Purpose                            |
| ----------- | ---------------------------------- |
| `fork()`    | Create a child process             |
| `execvp()`  | Execute external binary programs   |
| `waitpid()` | Wait for child process completion  |
| `chdir()`   | Change working directory           |
| `getcwd()`  | Retrieve current working directory |
| `getline()` | Read user input                    |
| `strtok()`  | Tokenize input arguments           |

---