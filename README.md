# Unix Shell

This is an implementation of a unix shell in C. The shell supports basic commands which can be executed with execvp function and some built in commands like cd and quit.

Execution:
- cd to the correct path
- make
- ./myshell or ./myshell batchfile

The shell has two modes:
- Interactive mode
- Batch mode

## Basic functions of code:
- main(int argc, char *argv[]) : main starts interactive or batch mode based on the number of arguments 

- void interactive() : the programm enters in a while loop and waits until user write a line .Then it calls parsing() to split the line in tokens and calls exec functions for all tokens.

- void batch(char *file) : reads the batchfile line by line with fgets function and calls parsing and exec functions for every line of the batchfile.

- int parsing(char line[512],char** ftoken,int *flags1) : Parses the line in tokens with strtok() function and delimeters ";" , "&&".When a token is after the characters && we make flags[i] equal to 1 .It saves the tokens in char** ftoken array and returns counter which is the number of commands in the line

- void exec(char line1[512],int k,int flags1[512],int *flags2) : splits the arguments of command(line[512]) with strtok() function and delimeter=" \t\r\n\a"(whitespace) ,saves the first token in array[0] and the next arguments in array[1],array[2],.. respectively. Then, it forks a child process; the child first checks if the flags1[k] and flags2[k-1] are both equal to 1, which means that the previous characters are && and the previous command failed and as a  result, the next command will not be executed. Then, the child executes the command check = execvp(array[0],array). If variable "check" is equal to -1 , execvp function failed and child informs his parent with a pipe that the command failed. The parent changes flags2[k] to 1. In any case the function kills the child to avoid zombies process.

