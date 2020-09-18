#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FORK_ERROR "ERROR: fork failed to execute."
#define FILE_ERROR "ERROR: file does not exist."

int processFile(char* filename);
int countNumberOfWords(char* string);
char* tokenizeString(char* string);

int main(int argc, const char * argv[]) {
  
    int i, returnedProcessFile;
    for(i=1; i<argc; i++){
        int cpid = fork();

        if(cpid == 0){
            //child process
            returnedProcessFile = processFile(argv[i]);

            if(returnedProcessFile == 0){ 
                printf("Child process %d for %s: number of words is %d.\n", cpid, argv[i], wordCount);
                return 0;
            }
            else{
                printf("Child process %d for %s: %s.\n", cpid, argv[i], FILE_ERROR);
                exit(1);
            }
        }
        else if(cpid == -1){
            fprintf(stderr, "%s\n", FORK_ERROR);
        }           
   }    
   

    //TODO: wait on all child processes that spawned.

 
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
    
    printf("%d total words.\n", totalWordCount);
    
    
    fclose(fp);
    
    
    return 0;
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
