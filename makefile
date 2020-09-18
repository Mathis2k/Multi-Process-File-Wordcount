wordcount: wordcount.o
	gcc -Wall -g -o wordcount wordcount.c
clean:
	rm -f wordcount
