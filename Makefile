all:programa

programa:main.o arvoreb.o
	gcc -o programa main.o arvoreb.o 

main.o:main.c
	gcc -o main.o main.c -c

arvoreb.o:arvoreb.c arvoreb.h
	gcc -o arvoreb.o arvoreb.c -c