#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include<sys/wait.h> 

#define WORD_LEN_MAX 100
#define LINE_LEN_MAX 512
#define NUMBER_OF_COMMAND 10

struct thread_args
{
  /* data */
  char *filename;
  int isCount;
  char *keyword;
  char *targetKeyword;
  char *sourceKeyword;
  int isAorB;
  char *insertedKeyword;
  char *outFile;
  int charCount;
  int lineCount;
  int startLine;
  int endLine;
  int mode;
};

struct command_args
{

  int startIndex;
  int endIndex;
  int totalNumberOfArgs;
  int mode;

};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

char commandArg[NUMBER_OF_COMMAND][WORD_LEN_MAX]; //holds all input arguments as an array seperated by spaces


void *search(); //search <keyword> [-c] <inFile>
//The command searches the input file for the occurrence of
//keyword and prints each line that contains the keyword. The The third argument
//"-c" is optional. If included, the command also prints the number of  times
//the keyword has been found by printing the number on the screen.

void *replace(); //replace <targetKeyword> <sourceKeyword> [-c] <inFile>
//replaces of all occurrences of targetKeyword with sourceKeyword.

void *insert(); //insert <insertedkeyword> [-c] <-a OR -b> <targetkeyword> <inFile>
// nsert command inserts the "insertedKeyword" before or after each occurence of
//the targetKeyword. -a (after) or -b (before) arguments decide whether the
//insertedKeyword is placed before or after the targetKeyword.

void *lineCount(); //lineCount <inFile>
// Counts the number of lines in the input File and prints it

void *split(); //split <CharacterCount> <inFile>
// Split command splits the lines in the input file into multiple lines.
//The output of the command is the same content as the input file, however
// each line cannot have more than "CharacterCount" characters. If a line
//within the input fi	le contains more characters, it is divided into multiple lines.

void *head(); //head <lineCount> <inFile>
// Prints the first "lineCount" lines in the input File

void *tail(); //tail <lineCount> <inFile>
// Prints the last "lineCount" lines in the input File

void *mid(); // mid <startLine> <endLine> <inFile>
// Prints the lines between "startline" and "endline" in the input File

// Helper functions:

void copyFile(); // copyFile <inFile> <outFile>
// Copies entire file to another one.

void midAlter();

void* commandProcess(void*ptr)
{
  struct command_args *args = (struct command_args *)ptr;
  int startIndex = args->startIndex;
  int endIndex = args->endIndex;
  int totalNumberOfArgs = args->totalNumberOfArgs;
  int mode = args->mode;

  int numberOfArgs = endIndex - startIndex;

  if (strcmp(commandArg[startIndex], "lineCount") == 0)
  {
    struct thread_args lineCountArgs;
    lineCountArgs.filename = commandArg[totalNumberOfArgs - 1];
    
    lineCount(&lineCountArgs);  
  }

  else if (strcmp(commandArg[startIndex], "split") == 0)
  {

    struct thread_args splitArgs;
    splitArgs.filename = commandArg[totalNumberOfArgs - 1];
    splitArgs.charCount = atoi(commandArg[startIndex + 1]);
    splitArgs.outFile = "temp.txt";
    splitArgs.mode = mode;

    if (numberOfArgs == 4)
    {
      splitArgs.outFile = commandArg[startIndex + 3];
    }

    split(&splitArgs);
    
  }

  else if (strcmp(commandArg[startIndex], "tail") == 0)
  {

    struct thread_args tailArgs;
    tailArgs.charCount = atoi(commandArg[startIndex + 1]);
    tailArgs.filename = commandArg[totalNumberOfArgs - 1];

    tail(&tailArgs);
    
  }

  else if (strcmp(commandArg[startIndex], "head") == 0)
  {
    
    struct thread_args headArgs;
    headArgs.charCount = atoi(commandArg[startIndex + 1]);
    headArgs.filename = commandArg[totalNumberOfArgs - 1];

    head(&headArgs);
  }

  else if (strcmp(commandArg[startIndex], "mid") == 0)
  {

    struct thread_args midArgs;
    midArgs.startLine = atoi(commandArg[startIndex + 1]);
    midArgs.endLine = atoi(commandArg[startIndex + 2]);
    midArgs.filename = commandArg[totalNumberOfArgs - 1];

    mid(&midArgs);
  }

  else if (strcmp(commandArg[startIndex], "search") == 0)
  {

    struct thread_args searchArgs;
    
    if (numberOfArgs == 2)
    {
      searchArgs.keyword = commandArg[startIndex + 1];
      searchArgs.filename = commandArg[totalNumberOfArgs - 1];
      searchArgs.isCount = 0;
    }
    else if (numberOfArgs == 3)
    {
      searchArgs.keyword = commandArg[startIndex + 1];
      searchArgs.filename = commandArg[totalNumberOfArgs - 1];
      searchArgs.isCount = 1;
    }

    search(&searchArgs);
  }

  else if (strcmp(commandArg[startIndex], "replace") == 0)
  {
    struct thread_args replaceArgs;
    replaceArgs.filename = commandArg[totalNumberOfArgs - 1];
    replaceArgs.targetKeyword = commandArg[startIndex + 1];
    replaceArgs.sourceKeyword = commandArg[startIndex + 2];
    replaceArgs.outFile = "temp.txt";
   

    if (numberOfArgs == 3)
    {
      replaceArgs.isCount = 0;
    }
    else if (numberOfArgs == 4)
    {
      replaceArgs.isCount = 1;
    }
    else if (numberOfArgs == 5)
    {
      replaceArgs.isCount = 0;
      replaceArgs.outFile = commandArg[startIndex + 4];
    }
    else if (numberOfArgs == 6)
    {
      replaceArgs.isCount = 1;
      replaceArgs.outFile = commandArg[startIndex + 5];
    }
    
    replace(&replaceArgs);
    
  }

  else if (strcmp(commandArg[startIndex], "insert") == 0)
  {

    struct thread_args insertArgs;
    insertArgs.filename = commandArg[totalNumberOfArgs - 1];
    insertArgs.outFile = "temp.txt";
   
    if (numberOfArgs == 4)
    {
     
      insertArgs.insertedKeyword = commandArg[startIndex + 1];
      insertArgs.targetKeyword = commandArg[startIndex + 3];
      insertArgs.isCount = 0;

      if (strcmp(commandArg[startIndex + 2], "-a"))
      {
        insertArgs.isAorB = 1;
      }
      else if (strcmp(commandArg[startIndex + 2], "-b"))
      {
        insertArgs.isAorB = 0;
      }
    }
    else if (numberOfArgs == 5)
    {
      if(endIndex==totalNumberOfArgs-1)
      {
        insertArgs.insertedKeyword = commandArg[startIndex + 1];
        insertArgs.targetKeyword = commandArg[startIndex + 3];
        insertArgs.isCount = 0;

        if (strcmp(commandArg[startIndex + 2], "-a"))
        {
          insertArgs.isAorB = 1;
        }
        else if (strcmp(commandArg[startIndex + 2], "-b"))
        {
          insertArgs.isAorB = 0;
        }

      }

      else
      {
        insertArgs.insertedKeyword = commandArg[startIndex + 1];
        insertArgs.targetKeyword = commandArg[startIndex + 4];
        insertArgs.isCount = 1;

        if (strcmp(commandArg[startIndex + 3], "-a"))
        {
          insertArgs.isAorB = 1;
        }
        else if (strcmp(commandArg[startIndex + 3], "-b"))
        {
          insertArgs.isAorB = 0;
        }
      }
    }

    else if (numberOfArgs == 6)
    { 

      if(endIndex == totalNumberOfArgs-1)//without output file
      {
        insertArgs.insertedKeyword = commandArg[startIndex + 1];
        insertArgs.targetKeyword = commandArg[startIndex + 4];
        insertArgs.isCount = 1;

        if (strcmp(commandArg[startIndex + 3], "-a"))
        {
          insertArgs.isAorB = 1;
        }
        else if (strcmp(commandArg[startIndex + 3], "-b"))
        {
          insertArgs.isAorB = 0;
        }

      }
      else//with output file
      {
        insertArgs.insertedKeyword = commandArg[startIndex + 1];
        insertArgs.targetKeyword = commandArg[startIndex + 3];
        insertArgs.isCount = 0;
        insertArgs.outFile = commandArg[startIndex + 5];

        if (strcmp(commandArg[startIndex + 2], "-a"))
        {
          insertArgs.isAorB = 1;
        }
        else if (strcmp(commandArg[startIndex + 2], "-b"))
        {
          insertArgs.isAorB = 0;
        }
      }
      
    }

    else if (numberOfArgs == 7)
    { // with outfile and -c
      insertArgs.insertedKeyword = commandArg[startIndex + 1];
      insertArgs.targetKeyword = commandArg[startIndex + 4];
      insertArgs.isCount = 1;
      insertArgs.outFile = commandArg[startIndex + 6];

      if (strcmp(commandArg[startIndex + 3], "-a"))
      {
        insertArgs.isAorB = 1;
      }
      else if (strcmp(commandArg[startIndex + 3], "-b"))
      {
        insertArgs.isAorB = 0;
      }
    }

    else if (numberOfArgs == 8)
    { // with outfile and -c
      insertArgs.filename = commandArg[startIndex + 5];
      insertArgs.insertedKeyword = commandArg[startIndex + 1];
      insertArgs.targetKeyword = commandArg[startIndex + 4];
      insertArgs.isCount = 1;
      insertArgs.outFile = commandArg[startIndex + 7];

      if (strcmp(commandArg[startIndex + 3], "-a"))
      {
        insertArgs.isAorB = 1;
      }
      else if (strcmp(commandArg[startIndex + 3], "-b"))
      {
        insertArgs.isAorB = 0;
      }
    }

      insert(&insertArgs);
  }

  else{
    printf("No such command exists. Please enter valid command.\n");
  }
}

int main(int argc, char *argv[])
{

  char input[LINE_LEN_MAX];

  printf("prompt:>");
  gets(input);

  while (strcmp(input, "quit") != 0)
  {

    if (strlen(input) == 0 || strlen(input) >= 512)
    {
      printf("incorrect input\n");
      printf("prompt:>");
      gets(input);
      continue;
    }

    int i = 0;
    int numberOfArgs = 0;

    char *token = strtok(input, " ");

    while (token != NULL)
    { //seperating input to string array seperated by spaces

      strcpy(commandArg[i++], token);
      numberOfArgs++;

      token = strtok(NULL, " ");
    }

    int numberOfColon = 0, numberOfSemiColon = 0;
    int colon[15], semicolon[15];
    int numberOfThreads = 0;

    for (int j = 0; j < numberOfArgs; j++)
    { //counting the colon and semi colon which is number of threads and storing numberOfArgses
      if (strcmp(commandArg[j], ":") == 0)
      {

        colon[numberOfColon++] = j;
      }
      else if (strcmp(commandArg[j], ";") == 0)
      {

        semicolon[numberOfSemiColon++] = j;
      }
    }
   

    numberOfThreads = numberOfColon + numberOfSemiColon + 1;
    pthread_t tids[numberOfThreads];
    ////////////////////////////////////////////////////////////////////////////////////
    // Single Sequential Mode
    ///////////////////////////////////////////////////////////////////////////////////

    if (numberOfColon == 0 && numberOfSemiColon == 0)
    {

      printf("Running in single sequential mode\n");
      int mode = 0;
      struct command_args command;
      command.startIndex = 0;
      command.endIndex = numberOfArgs-1;
      command.totalNumberOfArgs = numberOfArgs; 
      command.mode = 0;

      commandProcess(&command);

      printf("\n----------------------------------------\n");
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Multiple Sequential Mode (:)
    ///////////////////////////////////////////////////////////////////////////////////

    else if (numberOfColon > 0 && numberOfSemiColon == 0)
    {
      printf("Running in multiple sequential mode\n");

      int start = 0, end = colon[0];
      struct command_args commands [numberOfThreads];
      for (int i = 0; i < numberOfThreads; i++)
      {

        if (i >= numberOfColon)
        {
          end = numberOfArgs - 1;
        }
        if (i < numberOfColon)
        {
          end = colon[i];
        }


        commands[i].startIndex = start;
        commands[i].endIndex = end;
        commands[i].totalNumberOfArgs = numberOfArgs; 
        commands[i].mode = 0;
      
        commandProcess(&commands[i]);

        start = end + 1;
        printf("\n----------------------------------------\n");
      }
    }

    ////////////////////////////////////////////////////////////////////////////////////
    // Simultaneous / Threaded Mode (;)
    ///////////////////////////////////////////////////////////////////////////////////

    else if (numberOfSemiColon > 0 && numberOfColon == 0)
    {
      printf("Running in Threaded mode\n");

      struct command_args commands [numberOfThreads];
      int start = 0, end = semicolon[0];
      for (int i = 0; i < numberOfThreads; i++)
      {

        if (i >= numberOfSemiColon)
        {
          end = numberOfArgs - 1;
        }
        if (i < numberOfSemiColon)
        {
          end = semicolon[i];
        }

        
        commands[i].startIndex = start;
        commands[i].endIndex = end;
        commands[i].totalNumberOfArgs = numberOfArgs; 
        commands[i].mode = 2;
      
        
        int rc = pthread_create(&tids[i],NULL,commandProcess,&commands[i]);

        if(rc)			/* could not create thread */
        {
          printf("\n ERROR: return code from pthread_create is %d \n", rc);
          exit(1);
        }
        //printf("\n Created new thread (%lu) ... \n", tids[i]);
        
        start = end + 1;
        printf("\n----------------------------------------\n");
      }

      for (int i = 0; i < numberOfThreads; i++)
      {
        //printf("\n Joined thread (%lu) ... \n", tids[i]);
        pthread_join(tids[i], NULL);
      }
    }

    printf("\nprompt:>");
    gets(input);
  }

  return 0;
}

void *search(void *ptr)
{
  struct thread_args *args = (struct thread_args *)ptr;
  char *filename = args->filename;
  char *keyword = args->keyword;
  int isCount = args->isCount;


  FILE *fp;

  fp = fopen(filename, "r");
  char temp;

  if (fp == NULL)
  {
    printf("Failed to open the file %s", filename);
  }
  else
  {
    printf("Searching for keyword %s ...\n", keyword);
    printf("Found in following lines:\n\n");

    int wordCount = 0, lineCount = 0, i;
    char line[LINE_LEN_MAX], wordInText[WORD_LEN_MAX];

    while (fgets(line, sizeof(line), fp) != NULL)
    {
      lineCount++;
      char *p = line;
      while (*p != '\0' && *p != '\n')
      {
        i = 0;
        while (*p != ' ' && *p != '\0' && *p != '\n')
        {
          wordInText[i++] = (*p++);
        }
        if (*p == ' ')
        {
          p++;
        }
        wordInText[i] = '\0';

        if (!strcmp(wordInText, keyword))
        {
          wordCount++;
          printf("Line %d: %s\n", lineCount, line);
        }
      }
    }

    if (isCount == 1)
    {
      printf("\n%d occurences of keyword %s found in file %s\n", wordCount, keyword, filename);
    }
  }
  fclose(fp);

}
void *replace(void *ptr)
{

  struct thread_args *args = (struct thread_args *)ptr;
  char *filename = args->filename;
  char *targetKeyword = args->targetKeyword;
  char *sourceKeyword = args->sourceKeyword;
  int isCount = args->isCount;
  char *outFile = args->outFile;


  FILE *fp;
  FILE *fp_temp;

  fp = fopen(filename, "r+");
  fp_temp = fopen(outFile, "w+");
  int count = 0;

  if (fp == NULL)
  {
    printf("Failed to open the file %s", filename);
  }
  else if (fp_temp == NULL)
  {
    printf("Failed to open the temp file.");
  }
  else
  {

    char line[LINE_LEN_MAX];
    while (fgets(line, sizeof(line), fp) != NULL)
    {

      char *start = line, *end = NULL;

      while (1)
      {

        end = strstr(start, targetKeyword); //find the next occurence

        if (end == NULL)
        { // If there is no occurence left, copy whole text to temp file

          fwrite(start, 1, strlen(start), fp_temp);
          break;
        }
        else
        { // Occurence found and numberOfArgs stored at end pointer.
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
    if (strcmp(outFile, "temp.txt") == 0)
    {
      copyFile(fp, fp_temp);
    }

    printf("\nSuccessfully replaced all occurrences of '%s' with '%s'.\n", targetKeyword, sourceKeyword);

    if (isCount == 1)
    {
      printf("%d occurences changed.\n", count);
    }
  }

  fclose(fp);
  fclose(fp_temp);

}
void *insert(void *ptr)
{
  struct thread_args *args = (struct thread_args *)ptr;
  char *filename = args->filename;
  char *insertedKeyword = args->insertedKeyword;
  char *targetKeyword = args->targetKeyword;
  int isCount = args->isCount;
  int isAorB = args->isAorB;
  char *outFile = args->outFile;
  int mode = args->mode;

  if(strcmp(outFile, "temp.txt") == 0 && mode==2){
    pthread_mutex_lock(&mutex);
  }

  FILE *fp;
  FILE *fp_temp;

  fp = fopen(filename, "r+");
  fp_temp = fopen(outFile, "w+");
  int count = 0;

  if (fp == NULL)
  {
    printf("Failed to open the file %s", filename);
  }
  else if (fp_temp == NULL)
  {
    printf("Failed to open the temp file.");
  }
  else
  {

    char line[LINE_LEN_MAX];
    while (fgets(line, sizeof(line), fp) != NULL)
    {

      char *start = line, *end = NULL;

      while (1)
      {

        end = strstr(start, targetKeyword); //find the next occurence

        if (end == NULL)
        { // If there is no occurence left, copy whole text to temp file

          fwrite(start, 1, strlen(start), fp_temp);
          break;
        }
        else
        { // Occurence found and numberOfArgs stored at end pointer.
          count++;

          if (isAorB == 0)
          {

            fwrite(start, 1, end - start, fp_temp);
            fwrite(targetKeyword, 1, strlen(targetKeyword), fp_temp);
            fwrite(" ", 1, 1, fp_temp);
            fwrite(insertedKeyword, 1, strlen(insertedKeyword), fp_temp);
            start = end + strlen(targetKeyword);
          }
          else if (isAorB == 1)
          {

            fwrite(start, 1, end - start, fp_temp);
            fwrite(insertedKeyword, 1, strlen(insertedKeyword), fp_temp);
            fwrite(" ", 1, 1, fp_temp);
            fwrite(targetKeyword, 1, strlen(targetKeyword), fp_temp);
            start = end + strlen(targetKeyword);
          }
        }
      }
    }

    if (isAorB == 0)
    {
      printf("The word %s is inserted after %s", insertedKeyword, targetKeyword);
    }
    if (isAorB == 1)
    {
      printf("The word %s is inserted before %s", insertedKeyword, targetKeyword);
    }
    if (isCount == 1)
    {
      printf("%d times.", count);
    }
    printf("\n");
  }

  if (strcmp(outFile, "temp.txt") == 0)
  {
    copyFile(fp, fp_temp);
    if(mode==2)
    {
      pthread_mutex_unlock(&mutex);
    }
  }

  fclose(fp);
  fclose(fp_temp);

  /* if(mode==2){
    
  } */
}
void *lineCount(void *ptr)
{

  struct thread_args *args = (struct thread_args *)ptr;
  char *filename = args->filename;
  int mode = args->mode;

  int count = 1;
  FILE *fp;

  fp = fopen(filename, "r");
  char temp;

  if (fp == NULL)
  {
    printf("Failed to open the file %s", filename);
  }
  else
  {

    while ((temp = fgetc(fp)) != EOF)
    {
      if (temp == '\n')
        count++;
    }
  }
  fclose(fp);
  printf("The file %s has Line Count = %d\n", filename, count);
}

void *split(void *ptr)
{

  struct thread_args *args = (struct thread_args *)ptr;
  char *filename = args->filename;
  int charCount = args->charCount;
  char *outFile = args->outFile;
  int mode = args->mode;

  if(mode==2){
    pthread_mutex_lock(&mutex);
  }

  FILE *fp;
  FILE *fp_temp;

  fp = fopen(filename, "r+");
  fp_temp = fopen(outFile, "w+");
  int count = 0;

  if (fp == NULL)
  {
    printf("Failed to open the file %s", filename);
  }
  else if (fp_temp == NULL)
  {
    printf("Failed to open the temp file.");
  }

  else
  {

    int currentChar = 0;
    char word[WORD_LEN_MAX];
    char line[LINE_LEN_MAX];
    int currentLength = 0;

    int j;

    while (fgets(line, sizeof(line), fp) != NULL)
    {

      char *p = line;

      while (*p != '\0' && *p != '\n')
      {
        j = 0;
        while (*p != ' ' && *p != '\0' && *p != '\n')
        {
          word[j++] = (*p++);
        }
        if (*p == ' ')
        {
          p++;
        }
        word[j] = '\0';

        if (currentLength + strlen(word) > charCount)
        {

          fwrite("\n", 1, 1, fp_temp);
          fwrite(word, 1, strlen(word), fp_temp);
          fwrite(" ", 1, 1, fp_temp);
          currentLength = strlen(word) + 1;
        }
        else
        {

          fwrite(word, 1, strlen(word), fp_temp);
          fwrite(" ", 1, 1, fp_temp);
          currentLength += strlen(word) + 1;
        }
      }
    }

    if (strcmp(outFile, "temp.txt") == 0)
    {
      copyFile(fp, fp_temp);
    }

    fclose(fp);
    fclose(fp_temp);

    if(mode==2){
      pthread_mutex_unlock(&mutex);
    }
  }
}

void *head(void *ptr)
{

  struct thread_args *args = (struct thread_args *)ptr;
  char *filename = args->filename;
  int lineCount = args->charCount;
 

  FILE *fp;

  fp = fopen(filename, "r");
  char temp;

  if (fp == NULL)
  {
    printf("Failed to open the file %s", filename);
  }
  else if (lineCount == 0)
  {
    printf("lineCount has to be more than 0.\n");
  }

  else
  {
    int count = 0;
    char line[LINE_LEN_MAX];
    while (fgets(line, sizeof(line), fp) != NULL)
    {
      count++;
      if (count > lineCount)
      {
        break;
      }
      printf("%s", line);
    }
  }
}
void *tail(void *ptr)
{

  struct thread_args *args = (struct thread_args *)ptr;
  char *filename = args->filename;
  int lineCount = args->charCount;
 

  FILE *fp;

  fp = fopen(filename, "r");
  char temp;

  if (fp == NULL)
  {
    printf("Failed to open the file %s", filename);
  }

  else
  {

    int totalLines = 0;
    char line[LINE_LEN_MAX];
    while (fgets(line, sizeof(line), fp) != NULL)
    {
      totalLines++;
    }

    if (lineCount > totalLines)
    {
      printf("Given lineCount parameter is bigger then number of lines.\n");
    }
    else
    {
      int startLine = totalLines - lineCount;
      midAlter(startLine + 1, totalLines, filename);
    }
  }
  
}

void *mid(void *ptr)
{

  struct thread_args *args = (struct thread_args *)ptr;
  char *filename = args->filename;
  int startLine = args->startLine;
  int endLine = args->endLine;
  

  FILE *fp;

  fp = fopen(filename, "r");
  char temp;

  if (fp == NULL)
  {
    printf("Failed to open the file %s", filename);
  }
  else if (startLine == 0)
  {
    printf("startLine has to be more than 0.\n");
  }
  else if (startLine > endLine)
  {
    printf("Please enter valid range.\n");
  }

  else
  {
    int count = 0;
    char line[LINE_LEN_MAX];
    while (fgets(line, sizeof(line), fp) != NULL)
    {
      count++;
      if (count >= startLine && count <= endLine)
      {
        printf("%s", line);
      }
    }
  }
}

void copyFile(FILE *in, FILE *out)
{

  rewind(in);
  rewind(out);

  char line[LINE_LEN_MAX];

  while (fgets(line, LINE_LEN_MAX, out))
  {

    fwrite(line, 1, strlen(line), in);
  }
}

void midAlter(int startLine, int endLine, char filename[])
{

  FILE *fp;

  fp = fopen(filename, "r");
  char temp;

  if (fp == NULL)
  {
    printf("Failed to open the file %s", filename);
  }
  else if (startLine == 0)
  {
    printf("startLine has to be more than 0.\n");
  }
  else if (startLine > endLine)
  {
    printf("Please enter valid range.\n");
  }

  else
  {
    int count = 0;
    char line[LINE_LEN_MAX];
    while (fgets(line, sizeof(line), fp) != NULL)
    {
      count++;
      if (count >= startLine && count <= endLine)
      {
        printf("%s", line);
      }
    }
  }
}
