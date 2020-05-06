##############################
#Avaneesh Kolluri
#I pledge my honor that I have abided by the Stevens Honor System.
##############################

pfind: pfind.o phelp.o
	gcc pfind.o phelp.o -o pfind
pfind.o: pfind.c
	gcc -Wall -Werror -pedantic-errors -c pfind.c

phelp.o: phelp.c pfind.h
	gcc -Wall -Werror -pedantic-errors -c phelp.c
clean:
	rm -f *.o pfind pfind.exe