listhty: list.o my_getopt.o 
	gcc -Wall list.o my_getopt.o -o listhty

list.o: list.c my_getopt.h
	gcc -c -Wall list.c -o list.o

my_getopt.o: my_getopt.c my_getopt.h
	gcc -c -Wall my_getopt.c -o my_getopt.o

clean: 
	rm -rf *.o listhty