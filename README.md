# mini-shell


Nice, this is a solid project to own. Let’s strip it down and decide **exactly what commands + features your shell will support** before touching code.

Think in two layers:

* **Built-in commands** → you implement these yourself
* **External commands** → you just call real Linux programs via `execvp`

---

## 1. Core Built-in Commands (Must-Haves)

These are the “OS fundamentals” ones. I’d strongly recommend all of these:

1. **`exit`**

   * **Use:** `exit`
   * **What it does:** Terminates your shell.
   * **Why important:** Clean shutdown, maybe free memory, print a goodbye message.

2. **`cd`**

   * **Use:** `cd <dir>`, `cd ..`, `cd` (go to home)
   * **Syscall:** `chdir()`
   * **Why important:** Classic example of a command that **must** be built-in (a child process changing directory is useless for shell).

3. **`pwd`**

   * **Use:** `pwd`
   * **Syscall:** `getcwd()`
   * **Why important:** Simple and shows interaction with current working directory.

4. **`echo`**

   * **Use:** `echo hello world`
   * **Why important:** Easy to implement, good for testing arguments and parsing.

5. **`help`**

   * **Use:** `help`
   * **What it does:** Prints all supported commands and features.
   * **Why important:** Great for viva & demo, makes shell feel complete.

👉 These 5 give you a nice **basic shell personality**.

---

## 2. Extra Built-ins to Make It Yours

Pick **2–3** from here to give your shell its own flavor (and show OS depth):

6. **`clear`**

   * Just prints the ANSI escape code to clear the screen.
   * Makes demos look clean.

7. **`myshellinfo`** (or `about`)

   * Shows:

     * Your name, roll number
     * Shell features (piping, redirection, etc.)
   * Helps in viva + presentation instantly.

8. **`myps`** (simple custom `ps`)

   * Either:

     * Call external `ps` with some flags, or
     * (If you feel fancy later) read from `/proc` to show PID, name.
   * Directly ties to **process management** from your syllabus.

9. **`history`** (if you implement command storage)

   * Show last N commands (no need for arrow keys yet).
   * Great to demonstrate data structures + usability.

For now I’d suggest:

> **Final built-in set (initial plan):**
> `exit`, `cd`, `pwd`, `echo`, `help`, `clear`, `myshellinfo`
> (and maybe `history` later if you want)

---

## 3. External Commands Your Shell Should Comfortably Run

These you **don’t implement**, you just pass them to `execvp()`. But you should **test and demo** with a fixed set to show capability.

Your shell should be able to run things like:

* `ls`, `ls -l`, `ls -la`
* `cat file.txt`
* `touch file.txt`
* `mkdir testdir`
* `rm file.txt`
* `rmdir testdir`
* `cp a.txt b.txt`
* `mv a.txt b.txt`
* `grep "word" file.txt`
* `wc -l file.txt`
* `ps`, `top` (if terminal allows)
* `sleep 5`
* `whoami`, `date`

You don’t hardcode these; they’re just **evidence** that your shell integration with OS is correct.

---

## 4. Syntax Features (Not Commands, but Part of “Language” of Your Shell)

These are the **extra things** your shell will support *around* commands:

You earlier liked this combo: **Piping + I/O Redirection**. That’s solid.

### a) Piping (`|`)

Examples your shell should *aim* to support later:

* `ls -l | grep .c`
* `cat file.txt | sort | uniq | wc -l`

No new commands, just special syntax. This will use:

* `pipe()`
* multiple `fork()` calls
* `dup2()` to wire stdin/stdout

### b) I/O Redirection (`>`, `>>`, `<`)

Commands:

* `ls > out.txt`
* `echo "hello" >> log.txt`
* `sort < input.txt > output.txt`

You’ll parse redirection symbols and set up file descriptors using:

* `open()`, `creat()`
* `dup2()`
* `close()`

---

## 5. Locking It In: Concrete Command Set

Let’s write it like your **spec**.

### ✅ Built-in Commands (to implement yourself)

1. `exit` – exit the shell
2. `cd [dir]` – change directory
3. `pwd` – print current directory
4. `echo [args...]` – print text
5. `help` – show list of commands & features
6. `clear` – clear the terminal
7. `myshellinfo` – show project info (author, features, etc.)
8. (Optional later) `history` – show past commands

### ✅ External Commands (no implementation, just use for testing)

Your shell should be able to run commands like:

* `ls`, `cat`, `mkdir`, `rm`, `rmdir`, `cp`, `mv`, `grep`, `wc`, `sleep`, `ps`, `whoami`, `date`

### ✅ Syntax Features to Implement

* Simple commands: `cmd arg1 arg2 ...`
* **I/O Redirection:** `<`, `>`, `>>`
* **Piping:** `|`
* (Later optional) Background exec: `&`

---

If you’re cool with this set, next step is:

> **Next:** we design the *parsing model*:
> how we’ll break input like
> `ls -l | grep .c > out.txt`
> into structures (`Command`, `Pipeline`) and detect built-in vs external.

If you say “yes, this is the command set I’ll use,” I’ll help you design the **exact C structs and parsing flow** next.
