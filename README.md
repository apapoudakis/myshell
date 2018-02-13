# Unix Shell

This is an implementation of a unix shell in C. The shell supports basic commands which can be executed from execvp function and some built in commands like cd and quit.

Execution:
- cd to the correct path
- make
- ./myshell or ./myshell batchfile

The shell has two modes:
- Interactive mode
- Batch mode

## Basic functions of code:
- main() : main starts interactive or batch mode based on the number of arguments 

- interactive() : reads the command from the user

- batch() : reads the batchfile line by line 

- parsing() : Parse the line in tokens with strtok function and delimeter ; , &&

- exec() : fork a child process witch execute the commands

