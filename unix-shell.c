// Project 2: UNIX Shell
// Omar Mughal

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#define MAX_LINE 80 /* The maximum length command */
int main(void) {
    char line[MAX_LINE];
    char *args[MAX_LINE/2 + 1]; /* command line arguments */
    char *prev[MAX_LINE/2+1]; //previous command for ! !
    int argsLen=0;
    int should_run = 1; /* flag to determine when to exit program */
    while (should_run) {
        printf("osh>");
        fflush(stdout);
        /**
        * After reading user input, the steps are:
        * (1) fork a child process using fork()
        * (2) the child process will invoke execvp()
        * (3) parent will invoke wait() unless command included &
        */
        if(!fgets(line, MAX_LINE, stdin)){
            break;
        }
        bool hasAmp=false;
        if(strcmp(line, "&")>=0){ //if the input says "exit"
            hasAmp=true;
        }
        size_t length = strlen(line);
        if (line[length - 1] == '\n')
        line[length - 1] = '\0';
        if(strcmp(line, "exit")==0){ //if the input says "exit"
            return 0;
        }
        char *token;
        token=strtok(line," ");
        int tokIndex=0;
        for(;token!=NULL;tokIndex++){
            args[tokIndex]=token;
            token=strtok(NULL," ");
        }
        args[tokIndex]=NULL;
        argsLen=tokIndex;
        for(tokIndex=0; tokIndex<argsLen; tokIndex++){
            printf("%s\n", args[tokIndex]);      //print command/args
        }
        int procId=fork();
        if(procId<0){
            printf("Fork failed, exiting now.\n");
            return 0;
        }
        if(procId==0){
            execvp(args[0],args);
            printf("Child process ran\n");
        }
        else{
            if(hasAmp){
                sleep(2);
            }
            wait(NULL);
            printf("Child process terminated\n");
        }
        
    }
    return 0;
}