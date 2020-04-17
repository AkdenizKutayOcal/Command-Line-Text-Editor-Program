//  TO DO
// 1- add > functionality for all commands(added for insert, split, replace)
// 2- replace "cow" "smt" remove "" from strings
// 3- single sequential mode
// 4- threaded mode

// LOW PRIORITY
// fix tail method to not use midAlter
// make another mode to run commands with both : and ; 
// fix code inside main, It is a mess.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define WORD_LEN_MAX 100                                                       
#define LINE_LEN_MAX 512
#define NUMBER_OF_COMMAND 10

struct thread_args
{
  /* data */
  char* filename;
  int isCount;
  char* keyword;
  char* targetKeyword;
  char* sourceKeyword;
  int isAorB;
  char* insertedKeyword;
  char* outFile;
  int charCount;
  int lineCount;
  int startLine;
  int endLine;

};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *search();  //search <keyword> [-c] <inFile> 
//The command searches the input file for the occurrence of 
//keyword and prints each line that contains the keyword. The The third argument 
//"-c" is optional. If included, the command also prints the number of  times 
//the keyword has been found by printing the number on the screen.

void *replace(); //replace <targetKeyword> <sourceKeyword> [-c] <inFile>
//replaces of all occurrences of targetKeyword with sourceKeyword. 

void *insert();  //insert <insertedkeyword> [-c] <-a OR -b> <targetkeyword> <inFile>
// nsert command inserts the "insertedKeyword" before or after each occurence of 
//the targetKeyword. -a (after) or -b (before) arguments decide whether the 
//insertedKeyword is placed before or after the targetKeyword. 

void *lineCount(); //lineCount <inFile>
// Counts the number of lines in the input File and prints it

void *split();  //split <CharacterCount> <inFile>
// Split command splits the lines in the input file into multiple lines. 
//The output of the command is the same content as the input file, however
// each line cannot have more than "CharacterCount" characters. If a line 
//within the input fi	le contains more characters, it is divided into multiple lines.

void *head();  //head <lineCount> <inFile>
// Prints the first "lineCount" lines in the input File

void *tail();  //tail <lineCount> <inFile>
// Prints the last "lineCount" lines in the input File

void *mid();   // mid <startLine> <endLine> <inFile>
// Prints the lines between "startline" and "endline" in the input File

// Helper functions:

void copyFile();  // copyFile <inFile> <outFile>
// Copies entire file to another one.

void midAlter();

int main(int argc, char* argv[]){

  char input[LINE_LEN_MAX];
  char filename[50] = "file.txt"; 

  printf("prompt:>");
  gets(input);

  while(strcmp(input, "quit")!= 0) {

    int i = 0;
    int numberOfArgs = 0;
    char commandArg[NUMBER_OF_COMMAND][WORD_LEN_MAX]; //holds all input arguments as an array seperated by spaces

    char *token = strtok(input," ");
    
    while( token != NULL ) {    //seperating input to string array seperated by spaces
      
      strcpy(commandArg[i++],token);
      numberOfArgs ++;

      token = strtok(NULL," ");
    }

    int numberOfColon = 0,numberOfSemiColon = 0;
    int colon[15], semicolon[15];
    int numberOfThreads = 0;

    for(int j=0; j<numberOfArgs;j++){     //counting the colon and semi colon which is number of threads and storing indexes
      if(strcmp(commandArg[j],":")==0){
        
        colon[numberOfColon++] = j;
      }
      else if(strcmp(commandArg[j],":")==0){

        semicolon[numberOfSemiColon++];
      }
    }

    numberOfThreads = numberOfColon+numberOfSemiColon+1;

    pthread_t tids[numberOfThreads];
    

    ////////////////////////////////////////////////////////////////////////////////////
    // Single Sequential Mode
    ///////////////////////////////////////////////////////////////////////////////////


    if(numberOfColon==0 && numberOfSemiColon==0){
      printf("Running in single sequential mode\n");

      char filename [WORD_LEN_MAX];       //get filename which is last str
      strcpy(filename,commandArg[numberOfArgs-1]);




    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Multiple Sequential Mode (:)
    ///////////////////////////////////////////////////////////////////////////////////

    else if(numberOfColon>0 && numberOfSemiColon==0){  
      printf("Running in multiple sequential mode\n");

      char filename [WORD_LEN_MAX];       //get filename which is last str
      strcpy(filename,commandArg[numberOfArgs-1]);

      int start = 0, end = colon[0];
      for(int i=0;i<numberOfThreads;i++){
       
        char args[10][WORD_LEN_MAX];
        
        if(i>=numberOfColon){
          end = numberOfArgs-1;
        }
        if(i<numberOfColon){
          end = colon[i];
        }

        int index = 0;

        for(int k = start; k<end;k++){
          strcpy(args[index++],commandArg[k]);
        }

        //////////////////////////////////////////////////////////
        
        
        if(strcmp(args[0],"lineCount")==0){
          
          struct thread_args lineCountArgs;
          lineCountArgs.filename = filename; 
          
          pthread_create(&tids[i],NULL, lineCount,&lineCountArgs);
          pthread_join(tids[i],NULL);
        }

        else if(strcmp(args[0],"tail")==0){
          
          struct thread_args tailArgs;
          tailArgs.charCount = atoi(args[1]);
          tailArgs.filename = filename;

          pthread_create(&tids[i],NULL, tail,&tailArgs);
          pthread_join(tids[i],NULL);
        }

        else if(strcmp(args[0],"head")==0){
         
          struct thread_args headArgs;
          headArgs.charCount = atoi(args[1]);
          headArgs.filename = filename;

          pthread_create(&tids[i],NULL,head,&headArgs);
          pthread_join(tids[i],NULL);
        }

        else if(strcmp(args[0],"mid")==0){
         
          struct thread_args midArgs;
          midArgs.startLine = atoi(args[1]);
          midArgs.endLine = atoi(args[2]);
          midArgs.filename = filename;

          pthread_create(&tids[i],NULL,mid,&midArgs);
          pthread_join(tids[i],NULL);
        }

        else if(strcmp(args[0],"search")==0){
         
          struct thread_args searchArgs;

          if(index==2){
            searchArgs.keyword = args[1];
            searchArgs.filename = filename;
            searchArgs.isCount = 0;
          }
          else if(index == 3){
            searchArgs.keyword = args[1];
            searchArgs.filename = filename;
            searchArgs.isCount = 1;
          }
        
          pthread_create(&tids[i],NULL,search,&searchArgs);
          pthread_join(tids[i],NULL);
        }

        else if(strcmp(args[0],"replace")==0){
         
          struct thread_args replaceArgs;
          replaceArgs.filename = filename;
          replaceArgs.targetKeyword = args[1];
          replaceArgs.sourceKeyword = args[2];
          replaceArgs.outFile = "temp.txt";

          if(index==3){
            replaceArgs.isCount = 0;
          }
          else if(index == 4){
            replaceArgs.isCount = 1;
          }
          else if(index == 5){
            replaceArgs.isCount = 0;
            replaceArgs.outFile = args[4];
          }
          else if(index == 6){
            replaceArgs.isCount = 1;
            replaceArgs.outFile = args[5];
          }
        
          pthread_create(&tids[i],NULL,replace,&replaceArgs);
          pthread_join(tids[i],NULL);
        }

        else if(strcmp(args[0],"split")==0){
        
          struct thread_args splitArgs;
          splitArgs.filename = filename;
          splitArgs.charCount = atoi(args[1]);
          splitArgs.outFile = "temp.txt";

          if(index == 4){
            splitArgs.outFile = args[3];
          }

          pthread_create(&tids[i],NULL,split,&splitArgs);
          pthread_join(tids[i],NULL);
        }

        else if(strcmp(args[0],"insert")==0){
         
          struct thread_args insertArgs;
          insertArgs.filename = filename;
          insertArgs.outFile = "temp.txt";
          
          if(index==4){
            insertArgs.insertedKeyword = args[1];
            insertArgs.targetKeyword = args[3];
            insertArgs.isCount = 0;

            if(strcmp(args[2],"-a")){
              insertArgs.isAorB = 1;
            }
            else if(strcmp(args[2],"-b")){
              insertArgs.isAorB = 0;
            } 
          }
          else if(index == 5){
            insertArgs.insertedKeyword = args[1];
            insertArgs.targetKeyword = args[4];
            insertArgs.isCount = 1;

            if(strcmp(args[3],"-a")){
              insertArgs.isAorB = 1;
            }
            else if(strcmp(args[3],"-b")){
              insertArgs.isAorB = 0;
            } 
          }

          else if(index==6){  //with out file
            insertArgs.insertedKeyword = args[1];
            insertArgs.targetKeyword = args[3];
            insertArgs.isCount = 0;
            insertArgs.outFile = args[5];

            if(strcmp(args[2],"-a")){
              insertArgs.isAorB = 1;
            }
            else if(strcmp(args[2],"-b")){
              insertArgs.isAorB = 0;
            } 
          }

          else if(index == 7){  // with outfile and -c
            insertArgs.insertedKeyword = args[1];
            insertArgs.targetKeyword = args[4];
            insertArgs.isCount = 1;
            insertArgs.outFile = args[6];

            if(strcmp(args[3],"-a")){
              insertArgs.isAorB = 1;
            }
            else if(strcmp(args[3],"-b")){
              insertArgs.isAorB = 0;
            } 
          }
        
          pthread_create(&tids[i],NULL,insert,&insertArgs);
          pthread_join(tids[i],NULL);
        }

        /////////////////////////////////////////////////////////////////////////

        start = end+1;
        printf("\n----------------------------------------\n");
      }
    }
       
    
    printf("\nprompt:>");
    gets(input);
  }

  return 0;

}

void *search(void *ptr){
  // isCOunt == 0 when -c is not entered 1 otherwise

  struct thread_args *args = (struct thread_args *)ptr;
  char *filename = args->filename;
  char *keyword = args->keyword;
  int isCount = args->isCount;

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
  pthread_exit(NULL);
}
void *replace(void *ptr){

  struct thread_args *args = (struct thread_args *)ptr;
  char *filename = args->filename;
  char *targetKeyword = args->targetKeyword;
  char *sourceKeyword = args->sourceKeyword;
  int isCount = args->isCount;
  char *outFile = args->outFile;

  FILE*fp;
  FILE*fp_temp;

  // If output file is not given, lock critical section with mutex

  if(strcmp(outFile,"temp.txt")==0){
      pthread_mutex_lock(&mutex);
  }
    
  fp = fopen(filename,"r+");
  fp_temp = fopen(outFile,"w+");
  int count = 0;
  
  if(fp == NULL){
    printf("Failed to open the file %s",filename);

  }
  else if(fp_temp == NULL){
    printf("Failed to open the temp file.");

  }
  
  else{
    
    char line[LINE_LEN_MAX];
    while(fgets(line, sizeof(line), fp) != NULL){

      char *start = line, *end = NULL;
        
      while(1){

        end = strstr(start, targetKeyword); //find the next occurence

        if(end == NULL){ // If there is no occurence left, copy whole text to temp file

          fwrite(start, 1, strlen(start), fp_temp);
          break;
        }
        else{ // Occurence found and index stored at end pointer.
          count++;

          // Copy everything until the occurence
          fwrite(start, 1, end - start, fp_temp);
          
          // Write our replacement text
          fwrite(sourceKeyword, 1, strlen(sourceKeyword), fp_temp);

          // Next time, we want to start searching after our 'match'        
          start = end + strlen(targetKeyword);
        }
      }
    }
    if(strcmp(outFile,"temp.txt")==0){
      copyFile(fp,fp_temp);
      pthread_mutex_unlock(&mutex);
    }
    
    printf("\nSuccessfully replaced all occurrences of '%s' with '%s'.\n", targetKeyword, sourceKeyword);
    
    if(isCount == 1){
      printf("%d occurences changed.\n",count);
    }
  }
  
  fclose(fp);
  fclose(fp_temp);

  //remove("temp.txt");
  pthread_exit(NULL);
}
void *insert(void *ptr){  

  struct thread_args *args = (struct thread_args *)ptr;
  char *filename = args->filename;
  char *insertedKeyword = args->insertedKeyword;
  char *targetKeyword = args->targetKeyword;
  int isCount = args->isCount;
  int isAorB = args->isAorB;
  char *outFile = args->outFile;

  FILE*fp;
  FILE*fp_temp;

  // If output file is not given, lock critical section with mutex

  if(strcmp(outFile,"temp.txt")==0){
      pthread_mutex_lock(&mutex);
  }

  fp = fopen(filename,"r+");
  fp_temp = fopen(outFile,"w+");
  int count = 0;
  
  if(fp == NULL){
    printf("Failed to open the file %s",filename);

  }
  else if(fp_temp == NULL){
    printf("Failed to open the temp file.");

  }
  
  else{
   
    char line[LINE_LEN_MAX];
    while(fgets(line, sizeof(line), fp) != NULL){

      char *start = line, *end = NULL;
        
      while(1){

        end = strstr(start, targetKeyword); //find the next occurence

        if(end == NULL){ // If there is no occurence left, copy whole text to temp file

          fwrite(start, 1, strlen(start), fp_temp);
          break;
        }
        else{ // Occurence found and index stored at end pointer.
          count++;

          if(isAorB == 0){

            fwrite(start, 1, end - start, fp_temp);
            fwrite(targetKeyword, 1, strlen(targetKeyword), fp_temp);
            fwrite(" ", 1, 1, fp_temp);
            fwrite(insertedKeyword, 1, strlen(insertedKeyword), fp_temp);
            start = end + strlen(targetKeyword);
          }
          else if(isAorB == 1){

            fwrite(start, 1, end - start, fp_temp);
            fwrite(insertedKeyword, 1, strlen(insertedKeyword), fp_temp);
            fwrite(" ", 1, 1, fp_temp);
            fwrite(targetKeyword, 1, strlen(targetKeyword), fp_temp);
            start = end + strlen(targetKeyword);
          }
        }
      }
    }

    if(isAorB == 0){
      printf("The word %s is inserted after %s",insertedKeyword,targetKeyword);
    }
    if(isAorB == 1){
      printf("The word %s is inserted before %s",insertedKeyword,targetKeyword);
    }
    if(isCount == 1){
      printf("%d times.", count);
    }
    printf("\n");
  }

  if(strcmp(outFile,"temp.txt")==0){
      copyFile(fp,fp_temp);
      pthread_mutex_unlock(&mutex);
  }

  fclose(fp);
  fclose(fp_temp);
  pthread_exit(NULL);
}  
void *lineCount(void *ptr){
  // count +1 maybe
  
  struct thread_args *args = (struct thread_args *)ptr;
  char *filename = args->filename;
  
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
  pthread_exit(NULL);
}
int lineCountWithReturn(char filename[]){
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
  return count;

}

void *split(void *ptr){ 

  struct thread_args *args = (struct thread_args *)ptr;
  char *filename = args->filename;
  int charCount = args->charCount;
  char *outFile = args-> outFile;

  FILE*fp;
  FILE*fp_temp;

  // If output file is not given, lock critical section with mutex

  if(strcmp(outFile,"temp.txt")==0){
      pthread_mutex_lock(&mutex);
  }

  fp = fopen(filename,"r+");
  fp_temp = fopen(outFile,"w+");
  int count = 0;
  
  if(fp == NULL){
    printf("Failed to open the file %s",filename);

  }
  else if(fp_temp == NULL){
    printf("Failed to open the temp file.");

  }
  
  else{

    int totalLines = lineCountWithReturn(filename);
    int currentChar = 0;
    char word[WORD_LEN_MAX];
    char line[LINE_LEN_MAX];
    int currentLength = 0;
    

    int j;
   
    while(fgets(line, sizeof(line), fp) != NULL){  

      char *p = line;

      while (*p != '\0' && *p != '\n') {
        j = 0;
        while (*p != ' ' && *p != '\0' && *p != '\n') {                    
            word[j++] = (*p++);                               
        }                                                                  
        if (*p == ' ') {                                                   
            p++;                                                           
        }                                                            
        word[j] = '\0';
        
        if(currentLength+strlen(word)>charCount){

          fwrite("\n", 1, 1, fp_temp);
          fwrite(word,1,strlen(word),fp_temp);
          fwrite(" ", 1, 1, fp_temp);
          currentLength = strlen(word)+1;
          
        }
        else{
          
          fwrite(word,1,strlen(word),fp_temp);
          fwrite(" ", 1, 1, fp_temp);
          currentLength += strlen(word)+1;

        }           
      }
    }

    if(strcmp(outFile,"temp.txt")==0){
      copyFile(fp,fp_temp);
      pthread_mutex_unlock(&mutex);
    }
    
    fclose(fp);
    fclose(fp_temp);
    pthread_exit(NULL);
  }
}

void *head(void *ptr){

  struct thread_args *args = (struct thread_args *)ptr;
  char *filename = args->filename;
  int lineCount = args->charCount;

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
  pthread_exit(NULL);
}
void *tail( void *ptr){
  
  struct thread_args *args = (struct thread_args *)ptr;
  char *filename = args->filename;
  int lineCount = args->charCount;

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
      midAlter(startLine+1,totalLines,filename);
    }
  }
  pthread_exit(NULL);
}

void *mid(void *ptr){

  struct thread_args *args = (struct thread_args *)ptr;
  char *filename = args->filename;
  int startLine = args->startLine;
  int endLine = args->endLine;

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
  pthread_exit(NULL);
}

void copyFile(FILE *in, FILE *out){

    rewind(in);
    rewind(out);

    char line[LINE_LEN_MAX];

    while(fgets(line, LINE_LEN_MAX, out)){
      
      fwrite(line, 1, strlen(line), in);
    }
        
}

void midAlter(int startLine, int endLine, char filename[]){

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