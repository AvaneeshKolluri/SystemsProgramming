##############################
#Avaneesh Kolluri
#I pledge my honor that I have abided by the Stevens Honor System.
##############################

sort: quicksort.o sort.o
	gcc -Wall -Werror -pedantic-errors -g quicksort.o sort.o -o sort

sort.o: sort.c
	gcc -c sort.c

quicksort.o: quicksort.c quicksort.h
	gcc -c quicksort.c
clean:
	rm -f *.o sort sort.exe