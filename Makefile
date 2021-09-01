main.o:

	gcc -c main.c
	gcc -c file.c
	gcc file.o main.o -s -o small
	rm main.o
	rm file.o
	
	./small
