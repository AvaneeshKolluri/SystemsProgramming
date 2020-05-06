/*******************************************************************************
 * Name        : sort.c
 * Author      : Avaneesh Kolluri
 * Date        : 2/12/20
 * Description : Uses quicksort to sort a file of either ints, doubles, or
 *               strings.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quicksort.h"

#define MAX_STRLEN     64 // Not including '\0'
#define MAX_ELEMENTS 1024

typedef enum {
    STRING,
    INT,
    DOUBLE
} elem_t;

/**
 * Basic structure of sort.c:
 *
 * Parses args with getopt.
 * Opens input file for reading.
 * Allocates space in a char** for at least MAX_ELEMENTS strings to be stored,
 * where MAX_ELEMENTS is 1024.
 * Reads in the file
 * - For each line, allocates space in each index of the char** to store the
 *   line.
 * Closes the file, after reading in all the lines.
 * Calls quicksort based on type (int, double, string) supplied on the command
 * line.
 * Frees all data.
 * Ensures there are no memory leaks with valgrind. 
 */
int main(int argc, char **argv) {
    
	extern char *optarg;
	extern int optind, opterr, optopt;

	void display_usage(){
		printf("Usage: ./sort [-i|-d] [filename]\n"
		"   -i: Specifies the file contains ints.\n"
		"   -d: Specifies the file contains doubles.\n"
		"   filename: The file to sort.\n"
		"   No flags defaults to sorting strings.\n");
	}

	int opt;
	elem_t type = STRING;

	/*if(argc<2 || argc >3){
		display_usage();
		return EXIT_FAILURE;
	}*/
    
    int count = 0;

	while ((opt = getopt(argc, argv, ":id")) != -1)
	{
		switch (opt) 
	    {
	     case 'i':
	       type = INT;
            count++;
	       break;
	     case 'd':
	       type = DOUBLE;
            count++;
	       break;
	     case '?':
                //Take care of a case of the right flag and no file
            printf("Error: Unknown option '-%c' received.\n", optopt);
            display_usage();
            return EXIT_FAILURE;
	    }
	}
    if(argc == 1 || argc - 1 == count){
        display_usage();
        return EXIT_FAILURE;
    }
    
    if(argc-1 - count > 1){
        display_usage();
        return EXIT_FAILURE;
    }

	//open file
	//loop through and read the file
		//fgets() loop over it w while
		//strstr()
	//close the file

    FILE *fp;
	fp = fopen( argv[optind],"r");
	if(fp == NULL){
		printf("Error: Cannot open '%s'. %s.\n", argv[optind], strerror(errno));
        return EXIT_FAILURE;
	}

    char ** mem = (char **)malloc((MAX_ELEMENTS) * sizeof(char *));
    if (mem == NULL) {
        printf("Error opening file.");
        exit (EXIT_FAILURE);
    }


    int index = 0;
    char * eol;

    char str[MAX_STRLEN + 1];
    
    while(fgets(str, MAX_STRLEN + 2, fp) != NULL)
    {
        //printf("%s\n", "Test1");
        //printf("%s",str);
    	eol = strchr(str, '\n');
    	if(eol == NULL)
        {
    		str[MAX_STRLEN] = '\0';
        }else{
            *eol = '\0';
        }
    	
    	//Do a check to make sure its not empty line!! 
    	if(strlen(str) != 0)
        {
    		//Allocate space for string with malloc
    		*(mem + index) = (char *)malloc((MAX_STRLEN+2) * sizeof(char ));
    		strcpy(*(mem + index), str);
    		index++;
        }
        //printf("%s\n",str);
        
    	//printf("%s\n",str);
    }
    //printf("%s\n","test");

    
	fclose(fp);
    
    

	//if type is a string
	if(type == STRING){
		quicksort(mem, index, sizeof(char *), str_cmp);
        for (int i =0; i< index; i++){
            printf("%s\n", *(mem+i));
        }
	}
    
	//if type is an int
	else if(type == INT){
        int * temp = (int *)malloc((index) * sizeof(int ));
        
        for(int i =0; i < index; i++){
            temp[i] = atoi(*(mem+i));
        }
        //printf("%s\n", "Test1");
		quicksort(temp, index, sizeof(int), int_cmp);
        //printf("%s\n", "Test1");
        for (int i =0; i< index; i++){
            printf("%d\n", *(temp+i));
        }
        free(temp);
	}

	//if type is a double
	else if(type == DOUBLE){
        double * temp  = (double *)malloc((index) * sizeof(double ));
        
        for(int i =0; i < index; i++){
            if(*(mem+i) == NULL){
                break;
            }
            temp[i] = atof(*(mem+i));
        }
        
		quicksort(temp, index, sizeof(double), dbl_cmp);
        for (int i =0; i< index; i++){
            printf("%f\n", *(temp+i));
        }
        free(temp);
	}
     
    

	for(int i =0; i < index; i++){
		//Ask if we need to keep the array of characters or if we need to change the type
        free(*(mem+i));
	}
    //printf("%s\n", "hi");
	free(mem);
    return EXIT_SUCCESS;
}



//valgrind --leak-check=yes ./sort _____



