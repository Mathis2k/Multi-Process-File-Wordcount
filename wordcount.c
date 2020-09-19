#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define FORK_ERROR "ERROR: fork failed to execute."
#define FILE_ERROR "ERROR: file does not exist... exiting program."
#define WAIT_ERROR "ERROR: failed to wait for child."
#define UNKOWN_ERROR "ERROR: something went wrong... exiting program."
int totalProcesses = 0;
int totalUselessFiles = 0;
int totalSuccessfulFiles = 0;
int totalFiles = 0;

int processFile(char* filename);
int countNumberOfWords(char* string);
char* tokenizeString(char* string);

int main(int argc, const char * argv[]) {
  
    int i, retWordCount, status, retVal;

    printf("------------------ Running program ------------------\n");

    for(i=1; i<argc; i++){
        //int cpid = fork();
        retWordCount = processFile(argv[i]);
        
        if(retWordCount == -1)
            totalUselessFiles++;
        else{
            totalFiles++;
            int cpid = fork();
            if(cpid == 0){
                //child process            
                int pid = getpid();

                if(retWordCount != -1){ 
                    printf("Child process %d for %s: number of words is %d.\n", pid, argv[i], retWordCount);
                    return 0;
                }
                else{
                    printf("Child process %d for %s: %s.\n", pid, argv[i], FILE_ERROR);
                    exit(1);
                }
            }
            else if(cpid == -1){
                //fork failed
                fprintf(stderr, "%s\n", FORK_ERROR);
            }
        }           
   }    
   

    //TODO: wait on all child processes that spawned.

    for(; ; ){

        retVal = wait(&status);
        if(retVal == -1 && (errno == EINTR)){
            continue;
        }    
        else if(retVal == -1 && (errno == ECHILD)){
            break;
        }
        else if(retVal == -1){
            fprintf(stderr, "%s\n", WAIT_ERROR);
            break;
        }
        else if(WIFEXITED(status) && !WEXITSTATUS(status)){
            printf("Child %ld terminated normally.\n", (long)retVal);
            totalSuccessfulFiles++;
        }
        else
            fprintf(stderr, "%s", UNKOWN_ERROR);
       
        totalProcesses++; 
    }

    printf("------------------ Program Metadata ------------------\n");
    printf("Main process created %d child processes to count words in %d files.\n", totalProcesses, totalFiles);
    printf("%d files have been counted sucessfully.\n%d files did not exist.\n", totalSuccessfulFiles, totalUselessFiles);
 
    return 0;
}

int processFile(char *filename){
    
    char *line = NULL;
    int length;
    int read;
    
    int wordCount, totalWordCount = 0;
    
    FILE *fp = fopen(filename, "r");
    
    if(fp == NULL){
	    return -1;
    }
    
    while((read = getline(&line, &length, fp) != EOF)){
        
        if(*line != '\n'){
            line = tokenizeString(line); 
            wordCount = countNumberOfWords(line);
            totalWordCount += wordCount;

        }
        
        
    }
    
    fclose(fp);
    
    
    return totalWordCount;
}

char* tokenizeString(char* string){

    int sLength = strlen(string);

    char* newString = (char*)malloc(sLength);

    memset(newString, '\0', sLength);   
    
    strtok(string,"\n");

    char* pCh = strtok(string, " ");
    
    while(pCh != NULL){
        
        strcat(newString, pCh);
        strcat(newString," \0");
        pCh = strtok(NULL, " ");

    }

    newString[strlen(newString)] = '\0';
    return newString;
}

int countNumberOfWords(char* string){
    
    int i, wordCount = 0;
    
    for(i=0; i<strlen(string); i++){

        if(isspace(string[i])){
            wordCount++;

        }
    }
    
    return wordCount;
}
