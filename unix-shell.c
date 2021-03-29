// Project 2: UNIX Shell
// Omar Mughal

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_LINE 80 /* The maximum length command */


int findAmp(char str[], int n) {
	int i = 0;
	for (; i < n; ++i){
      if(str[i] == '&'){
    	  return i;
      }
   }
   return i;
}

char *trimString(char *s) {
    char *ptr;
    if (!s)
        return NULL;   // handle NULL string
    if (!*s)
        return s;      // handle empty string
    for (ptr = s + strlen(s) - 1; (ptr >= s) && isspace(*ptr); --ptr);
    ptr[1] = '\0';
    return s;
}


int main(void) {
    char line[MAX_LINE] = "\0";
    int lineLen=strlen(line);
    char prevLine[MAX_LINE] = "\0";
    char *args[MAX_LINE/2 + 1]; /* command line arguments */
    char *prev[MAX_LINE/2+1]; //previous command for ! !
    int argsLen=0;
    int should_run = 1; /* flag to determine when to exit program */
    int fd[2];
    pipe(fd);
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
        bool hasPipe=false;

        //int ampIndex = strchr(line, '&');

        int ampIndex = findAmp(line, strlen(line));

        if(ampIndex){
            hasAmp=true;
            for (int i = ampIndex; i < strlen(line); i++ ) {
            	line[i] = '\0';
            }
        }

        char *locationOfGreaterThan = strchr(line, '>');
        if(locationOfGreaterThan != NULL ){
            //for(int i=0;i<argsLen;i++){
                //if(strchr(args[i],'>')){
                    printf("File name is %s\n", trimString(locationOfGreaterThan+1) );
                    //int fd=open(args[i+1],O_RDWR);
                    //dup2(fd, STDOUT_FILENO);
                //}
            //}
        }
        else if(strchr(line, '<')){
            for(int i=0;i<argsLen;i++){
                if(strchr(args[i],'<')){
                    int fd=open(args[i+1],O_RDWR);
                    read(fd,line,BUFSIZ*sizeof(char));
                }
            }
        }
        if(strchr(line,'|')){
            hasPipe=true;
        }
        size_t length = strlen(line);
        if (line[length - 1] == '\n')
        	line[length - 1] = '\0';

        if(!strcmp(line, "exit")){ //if the input says "exit"
            return 0;
        }

        if(strncmp(line,"!!",2)==0){
            if(strlen(prevLine)==0){
                fprintf(stderr,"No previous command\n");
            }
            printf("Previous command: %s\n",prevLine);
            strcpy(line,prevLine);

        }
        strcpy(prevLine,line);
        printf("You entered: %s\n",line);


        char *token;
        token=strtok(line," ");
        int tokIndex=0;
        for(;token!=NULL;tokIndex++){
            args[tokIndex]=token;
            token=strtok(NULL," ");
        }
        /*if(hasAmp==true){
            tokIndex--;
        }*/
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