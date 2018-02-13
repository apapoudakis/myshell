# Unix Shell

This is an implementation of a unix shell in C.

Execution:
- cd to the correct path
- make
- ./myshell or ./myshell batchfile

The shell has two modes:
- Interactive mode
- Batch mode

## Basic functions of code:
- interactive() : reads the command from the user

- batch() : reads the batchfile line by line 

- parsing() : Parse the line in tokens with strtok function and delimeter ; and &&

- exec() : fork a child process witch execute the commands

