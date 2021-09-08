main.o:

	gcc -c main.c
	gcc -c file.c
	gcc -c util.c
	gcc util.o file.o main.o -s -o small -nostdinc
	rm main.o
	rm file.o
	rm util.o
	
	./small
