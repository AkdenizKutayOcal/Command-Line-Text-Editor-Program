#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD_LEN_MAX 100                                                       
#define LINE_LEN_MAX 500

void search();  //search <keyword> [-c] <inFile> 
//The command searches the input file for the occurrence of 
//keyword and prints each line that contains the keyword. The The third argument 
//"-c" is optional. If included, the command also prints the number of  times 
//the keyword has been found by printing the number on the screen.

void replace(); //replace <targetKeyword> <sourceKeyword> [-c] <inFile>
//replaces of all occurrences of targetKeyword with sourceKeyword. 

void insert();  //insert <insertedkeyword> [-c] <-a OR -b> <targetkeyword> <inFile>
// nsert command inserts the "insertedKeyword" before or after each occurence of 
//the targetKeyword. -a (after) or -b (before) arguments decide whether the 
//insertedKeyword is placed before or after the targetKeyword. 

void lineCount(); //lineCount <inFile>
// Counts the number of lines in the input File and prints it

void split();  //split <CharacterCount> <inFile>
// Split command splits the lines in the input file into multiple lines. 
//The output of the command is the same content as the input file, however
// each line cannot have more than "CharacterCount" characters. If a line 
//within the input fi	le contains more characters, it is divided into multiple lines.

void head();  //head <lineCount> <inFile>
// Prints the first "lineCount" lines in the input File

void tail();  //tail <lineCount> <inFile>
// Prints the last "lineCount" lines in the input File

void mid();   // mid <startLine> <endLine> <inFile>
// Prints the lines between "startline" and "endline" in the input File

int main(int argc, char* argv[]){

  char input[50];
  char command[50];
  char *inputptr;
  char filename[50] = "file.txt"; 
  int i;

  inputptr = input;

  printf("prompt:>");
  gets(inputptr);

  while(strcmp(inputptr, "quit")!= 0) {

    printf("LineCount:\n");
    lineCount(filename);

    printf("Search:\n");
    search("Lorem",1, filename);

    printf("Head:\n");
    head(0,filename);

    printf("Mid\n");
    mid(3,5,filename);

    printf("Tail:\n");
    tail(14,filename);

    printf("\nprompt:>");
    gets(inputptr);
  }

  return 0;

}

void search(char keyword[], int isCount, char filename[]){
  // isCOunt == 0 when -c is not entered 1 otherwise

  FILE*fp;

  fp = fopen(filename,"r");
  char temp;
  
  if(fp == NULL){
    printf("Failed to open the file %s",filename);

  }
  else{
    printf("Searching for keyword %s ...\n",keyword);
    printf("Found in following lines:\n\n");

    int wordCount = 0, lineCount = 0,i;
    char line[LINE_LEN_MAX], wordInText[WORD_LEN_MAX];

    while(fgets(line, sizeof(line), fp) != NULL){
      lineCount++;
      char *p = line;
      while (*p != '\0' && *p != '\n') {
        i = 0;
        while (*p != ' ' && *p != '\0' && *p != '\n') {                    
                wordInText[i++] = (*p++);                               
            }                                                                  
            if (*p == ' ') {                                                   
                p++;                                                           
            }                                                                  
            wordInText[i] = '\0';
            
            if(!strcmp(wordInText, keyword)){
                wordCount++;
                printf("Line %d: %s\n",lineCount, line);

            }
      }
    }

    if(isCount == 1){
      printf("\n%d occurences of keyword %s found in file %s\n",wordCount, keyword ,filename);
    }

  }
  fclose(fp);

}

void lineCount(char filename[]){
  // count +1 maybe

  int count = 1;
  FILE*fp;

  fp = fopen(filename,"r");
  char temp;
  
  if(fp == NULL){
    printf("Failed to open the file %s",filename);

  }
  else{

      while((temp = fgetc(fp))!=EOF) {
        if(temp == '\n')
          count++;
      }
  
  }
  fclose(fp);
  printf("The file %s has Line Count = %d\n", filename, count);

}

void head(int lineCount, char filename[]){

  FILE*fp;

  fp = fopen(filename,"r");
  char temp;
  
  if(fp == NULL){
    printf("Failed to open the file %s",filename);
  }
  else if(lineCount==0){
    printf("lineCount has to be more than 0.\n");
  }

  else{
    int count = 0;
    char line[LINE_LEN_MAX];
    while(fgets(line, sizeof(line), fp) != NULL){
      count++;
      if(count>lineCount){
        break;
      }
      printf("%s",line);

    }
  }
}
void tail(int lineCount, char filename[]){
  
  FILE*fp;

  fp = fopen(filename,"r");
  char temp;
  
  if(fp == NULL){
    printf("Failed to open the file %s",filename);
  }

  else{

    int totalLines = 0;
    char line[LINE_LEN_MAX];
    while(fgets(line, sizeof(line), fp) != NULL){
      totalLines++;
    }

    if(lineCount>totalLines){
      printf("Given lineCount parameter is bigger then number of lines.\n");

    }
    else{
      int startLine = totalLines-lineCount;
      mid(startLine+1,totalLines,filename);
    }
  }

}
void mid(int startLine, int endLine, char filename[]){

  FILE*fp;

  fp = fopen(filename,"r");
  char temp;
  
  if(fp == NULL){
    printf("Failed to open the file %s",filename);
  }
  else if(startLine==0){
    printf("startLine has to be more than 0.\n");
  }
  else if(startLine>endLine){
    printf("Please enter valid range.\n");
  }

  else{
    int count = 0;
    char line[LINE_LEN_MAX];
    while(fgets(line, sizeof(line), fp) != NULL){
      count++;
      if(count>=startLine && count<=endLine){
        printf("%s",line);
      }
    }
  }
}