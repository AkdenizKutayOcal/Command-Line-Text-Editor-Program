editor: editor.o
	gcc editor.c -o editor -lpthread 

editor.o: editor.c
	gcc -O -c editor.c
files: 
	$(file > commands.txt)
	$(file > myFile.txt)
clean:
	rm -rf *o editor editor.o myFile.txt command.txt
