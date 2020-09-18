#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wordcount.h"

#define BUFFER_SIZE 1024;
#define NULL_FILE_POINTER_ERROR "ERROR: file pointer returned NULL."


int processFile(char* filename);
char* removeLeadingDelims(char* string);
char* removeDelims(char* string);

int main(int argc, const char * argv[]) {
  
    /* TODO:
     *      // control process (in notes)
     *      // add null file check
     *
     */
    
    
    
    
    
    processFile(argv[1]);
    
    return 0;
}

int processFile(char *filename){
    
    int buffSize = BUFFER_SIZE;

    char *line = NULL;
    int length;
    int read;
    
    int wordCount, totalWordCount = 0;
    
    FILE *fp = fopen(filename, "r");
    
    if(fp == NULL){
        
        fprintf(stderr, "%s\n", NULL_FILE_POINTER_ERROR);

    }
    
    while((read = getline(&line, &length, fp) != EOF)){
        
        if(*line != '\n'){
            //line = removeLeadingDelims(line);
            line = removeDelims(line); 
            wordCount = countNumberOfWords(line);
            totalWordCount += wordCount;

        }
        
        
    }
    
    printf("%d total words.\n", totalWordCount);
    
    
    fclose(fp);
    
    
    return 0;
}

char* removeDelims(char* string){

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
