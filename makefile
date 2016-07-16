smsh : smsh.o
	gcc -o smsh smsh.o
smsh.o : smsh.c
	gcc -c smsh.c
