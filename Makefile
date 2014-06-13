main : main.o func.o
	gcc -lm -o main main.o func.o
main.o : main.c init.h
	gcc -c main.c
func.o : func.c init.h
	gcc -c func.c
