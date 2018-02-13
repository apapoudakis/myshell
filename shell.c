#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>


void exec(char line1[512],int k,int flags1[512],int *flags2);
int parsing(char line[512],char **ftoken,int *flags1);
void interactive();
void batch(char *file);
char *strtokm(char *str, const char *delim);

int main(int argc, char *argv[])
{
    
    if (argc==1)
    {
        interactive();//interactive mode
    }
    else
    {
        batch(argv[1]);//batch mode
    }
    
    return 0;
}

void interactive()
{
    int i,pid,status,compare,n=0,counter=0;
    size_t lineSize=512;
    char line[512];
    char *token2;
    int flags1[100];
    int flags2[100];
    
   
    do
    {
        
        char **token,**ftoken;
        token = (char **)malloc(sizeof(char)*512);
        ftoken = (char **)malloc(sizeof(char)*512);
        
        for (i=0;i<100;i++)
        {
            flags1[i] = 0;
            flags2[i] = 0;
        }
       
        
        printf("papoudakis_8600> ");
        
        fgets(line,512,stdin); //read line from standard input
        
        // parse line different commands (;,&&)  
        counter = parsing(line,ftoken,flags1); //counter: number of commands in the line
        
        for (i=0;i<counter;i++)
        {
            
            token2 = ftoken[i];
            exec(token2,i,flags1,flags2); //execute a command
            
        }
        
        free(token);
        free(ftoken);
        
        
    }while(1);
    
}
void batch(char *file)
{

    FILE *fp;
    fp = fopen(file,"r");
    char line[512];
    int i,n=0,counter=0;
    char *token2;
    int flags1[100];
    int flags2[100];
    
    //read the batchfile line by line
    while(fgets(line,512,fp))
    {
        char **token,**ftoken;
        token = (char **)malloc(sizeof(char)*512);
        ftoken = (char **)malloc(sizeof(char)*512);
        
        for (i=0;i<100;i++)
        {
            flags1[i] = 0;
            flags2[i] = 0;
        }
        // parse line different commands (;,&&)
        counter = parsing(line,ftoken,flags1); //counter: number of commands in the line
        
        for (i=0;i<counter;i++)
        {
            
            token2 = ftoken[i];
            exec(token2,i,flags1,flags2);//execute a command
            
        }
        free(token);
        free(ftoken);
        
    }
    
    
    fclose(fp);
    
}

void exec(char line1[512],int k,int flags1[512],int *flags2)
{
    int buf=0;
    int i,check=0,pid,status,compare,n=0;
    char *nul;
    char *token;
    token = NULL;
    
    char **array;
    array = (char **)malloc(sizeof(char)*512);
    for (i=0;i<50;i++)
    {
        array[i] = NULL;
    }
    array[0] = NULL;
    
    const char delimeter[10] = "  \t\r\n\a";//whitespace delimeter
    
    // get the first token
    token = strtok(line1, delimeter);
    array[0] = token;
    
    
    if (token!=NULL)
    {
        compare = strcmp(token,"quit");
        if (!compare)
        {
            exit(0);
        }
    }
    
    n++;
    
    // walk through other tokens 
    while( token != NULL ) 
    {
        
        token = strtok(NULL, delimeter);
        array[n] = token;
        n++;
    }
    
    array[n] = NULL;
    
    //check for cd command 
    if (strcmp(array[0],"cd")==0)
    {
        if ((flags1[k]!=1) || (flags2[k-1]!=1))
        {
            check = chdir(array[1]);
        }
        
    }
    
    int pd[2];
    //create a pipe for communication between child and father
    if( pipe(pd) < 0 )
    { 
        printf("cant open pipe");
    }
    
    pid = fork();
    
    wait(&status);//parent wait child process to finish
    
    if ( pid == 0 )
    { 
        //Child process 
        if (k!=0)
        { 
            //if previous command failed and there is && between commands kill the child
            if ((flags1[k]==1) && (flags2[k-1]==1))
            {
                close(pd[0]);//close the reading end of pipe
                buf = 1;
                write(pd[1],&buf,sizeof(buf));//child send value 1 means that execvp failed
                close(pd[1]);//close the writing end of pipe
                kill(getpid(), SIGKILL);
            }
        }
        
        if (strcmp(array[0],"cd") != 0)
        {
            check = execvp(array[0],array);
        }
        
        if (check==-1)
        {
            //execvp failed
            printf("command not found: %s \n",array[0]);
            close(pd[0]);//close the reading end of pipe
            buf = 1;
            write(pd[1],&buf,sizeof(buf));//child send value 1 means that execvp failed
            close(pd[1]);//close the writing end of pipe
        }
        kill(getpid(), SIGKILL);//kill the child to avoid zombie process
        
    } 
    close(pd[1]);//close the writing end of pipe
    read(pd[0],&buf,sizeof(buf));
    
    if (buf==1)
    {
        flags2[k] = 1;
    }
    close(pd[0]);//close the reading end of pipe
    
    

    free(array);
    
}
//parse the line between ;,&&
int parsing(char line[512],char **ftoken,int *flags1)
{
    int n=0,counter=0,i;
    
    const char delimeter1[10] = ";";
    const char delimeter2[10] = "&&";
    
    char **token;
    token = (char **)malloc(sizeof(char)*512);
    
    char *token2;
    //first chech for ; characters
    token2 = strtok(line, delimeter1);
    token[n] = token2;

    n++;
    
    while( token2 != NULL ) 
    {
        token2 = strtok(NULL, delimeter1);
        
        if (token2==NULL)
        {
            break;
        }
        token[n] = token2;
        
        n++;
    }
    
    counter = 0;
    //then check for &&
    for (i=0;i<n;i++)
    {
        token2 = strtokm(token[i],delimeter2);//we use strokm to avoid only one & parse the line
        ftoken[counter] = token2;
        
        counter++;
        
        while( token2 != NULL ) 
        {
            token2 = strtokm(NULL, delimeter2);//we use strokm to avoid only one & parse the line
        
            if (token2==NULL)
            {
                break;
            }
            flags1[counter] = 1;//before the command there is && character
            ftoken[counter] = token2;
        
            counter++;
        }
        
    }
    //return the number of commands in the line
    return counter;
        
}
//this functions works like strok() but parse the line only with delimeter && not  & 
char *strtokm(char *str, const char *delim)
{
    static char *tok;
    static char *next;
    char *m;

    if (delim == NULL) return NULL;

    tok = (str) ? str : next;
    if (tok == NULL) return NULL;

    m = strstr(tok, delim);

    if (m) {
        next = m + strlen(delim);
        *m = '\0';
    } else {
        next = NULL;
    }

    return tok;
}
