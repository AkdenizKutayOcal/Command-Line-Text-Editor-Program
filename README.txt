This project is made by Akdeniz Kutay Ocal for the CMPE_382 course.

1-Design Overview:

Project is designed to be a basic Command Line Text Editor program which is written 
in C language and can run in Linux systems. There are some functionalities that 
expects user input and executes on a command line interpreter or shell according to
that. These basic functionalities are as follows;

search <keyword> [-c] <inFile> :
    The command searches the input file for the occurrence of
    keyword and prints each line that contains the keyword. The The third argument
    "-c" is optional. If included, the command also prints the number of  times
    the keyword has been found by printing the number on the screen.
    NOTE: without ""'s

replace <targetKeyword> <sourceKeyword> [-c] <inFile> :
    replaces of all occurrences of targetKeyword with sourceKeyword.

insert <insertedkeyword> [-c] <-a OR -b> <targetkeyword> <inFile>:
    Insert command inserts the "insertedKeyword" before or after each occurence of
    the targetKeyword. -a (after) or -b (before) arguments decide whether the
    insertedKeyword is placed before or after the targetKeyword.

lineCount <inFile>:
    Counts the number of lines in the input File and prints it

split <CharacterCount> <inFile>:
    Split command splits the lines in the input file into multiple lines.
    The output of the command is the same content as the input file, however
    each line cannot have more than "CharacterCount" characters. If a line
    within the input file contains more characters, it is divided into multiple lines.

head <lineCount> <inFile>:
    Prints the first "lineCount" lines in the input File

tail <lineCount> <inFile>:
    Prints the last "lineCount" lines in the input File

mid <startLine> <endLine> <inFile>:
    Prints the lines between "startline" and "endline" in the input File


These functionalities can be run in three different execution modes;

1- You can run single commands.

    prompt> mid 100 200 myFile.txt
    prompt> split 100 myFile.txt > out.txt
    prompt> replace Lorem Ipsum -c myFile.txt //without ""'s

2- You can run multiple commands in a single line in a sequential mode
   by seperating your commands by ":" and writing the filename at the end.

    prompt> mid 100 200 : lineCount myFile.txt
    prompt> tail 5 : insert Ipsum -a Lorem > out.txt : lineCount myFile.txt


3- You can run multiple commands in a single line in a threaded mode
   by seperating your commands by ";" and writing the filename at the end.

    prompt> head 100 ; lineCount myFile.txt 
    prompt> mid 20 40 ; tail 8 ; split 80 myFile.txt

NOTE: You can't run ":" and ";" commands at the same time.

Additional Functionalities:

1- You can store the changes after using Replace, Split Insert commands in a 
   different file while not changing the original one using "> outFile".

    prompt> split 100 myFile.txt > out.txt
    prompt> replace Lorem Ipsum myFile.txt > out.txt : insert Ipsum -a Lorem > out2.txt

2- Complete Specification:

The code works as a loop. It takes a user input and seperating it by whitespace tokens and 
generates a commandArg string array. While doing that it checks if the input argument is empty
or bigger than 512 chars. Than it counts the number of semicolon and colon characters and stores
the indexes of them. 

The code has 3 execution mode as it is written previously. By checking the number of semicolon and colon
in the user input, commandPrecess function is called to further seperate the input and call for appropiate
function. However, when it is in threaded mode, instead of doing method call, a pthread is created for all
commands and after creation of all threads finished, they all joined together. 


3- Known bugs or problems:

I haven't been writing C code for a long time and using high level languages. Therefore, some parts
of the code is poorly written and there are some bad code practices. That's why this code probably
have some bugs that I haven't discovered yet. Since I spend so much time trying to find foolish errors
of mine, I did not have time to fix some problems eventhough I was aware of them. Here are some;

->  I did not have time to check all errors so if user enters extreme inputs it will probably
    get segmentation fault. But I check errors for empty input, very large input, incorrect filename,
    incorrect command argument, no argument between semicolon\colon etc.

->  No check for double whitespaces.

->  After I put mutexes inside insert function, and added output operations, it stopped working for 
    single sequential insert with count. Will be fixed after submission.

->  In threaded mode when following command line is given, insert command is not executed. I suppose it is 
    because of the fact that they both trying to write to the same file. I did not have time to correctly
    execute mutex operation for replace. Eventhough it would probably work when I lock all threads, it 
    would work like a sequential mode therefore I did not put it.

->  Tail function uses unnecessary MidAlter function which should be removed.

->  ">" functionality only available for replace, insert and split commands.
