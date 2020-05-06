/*******************************************************************************
 * Name        : pfind.c
 * Author      : Avaneesh Kolluri
 * Date        : 2/24/20
 * Description : HW3 main method code.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <errno.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include<errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include "pfind.h"

void display_usage(){
    printf("Usage: ./pfind -d <directory> -p <permissions string> [-h]\n");
}

int main(int argc, char **argv) {
    
	extern char *optarg;
	extern int optind, opterr, optopt;
	
    //char *perm_str[10];
    int count = 0;
    int d = 0;
    //int h;
    int p = 0;
    int opt;
    char * dir_str;
    char * perm_str;
    
	while ((opt = getopt(argc, argv, ":d:p:h")) != -1)
	{
		switch (opt) 
	    {
	     case 'd':
            
            dir_str = (char *)malloc(PATH_MAX * sizeof(char));
            
            strcpy(dir_str, optarg);
                
            
            //dir_str = optarg;
            
            d++;
            count++;
	       break;
	     case 'h':
            display_usage();
            return EXIT_SUCCESS;
         case 'p':
            perm_str = (char *)malloc(10 * sizeof(char));
            //printf("%p\n", optarg);
            strcpy(perm_str, optarg);
            //perm_str = optarg;
            p++;
            count++;
            break;
	     case '?':
                //Take care of a case of the right flag and no file
            fprintf(stderr, "Error: Unknown option '-%c' recieved.\n", optopt);
             return (EXIT_FAILURE);
        default:
            break;
	    }
	}
    
    //printf("%s\n", dir_str);
   // printf("test\n");
    
    if(argc == 1){
        display_usage();
        return (EXIT_FAILURE);
    }
    if(d==0){
        printf("Error: Required argument -d <directory> not found.\n");
        return EXIT_FAILURE;
    }
    if(p == 0){
        printf("Error: Required argument -p <permissions string> not found.\n");
        return EXIT_FAILURE;
    }
    
    
    if(strlen(perm_str) != 9){
        fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", perm_str);
        return EXIT_FAILURE;
    }
    
    //int permission_valid;
    int perms[] = {S_IRUSR, S_IWUSR, S_IXUSR,
    S_IRGRP, S_IWGRP, S_IXGRP,
    S_IROTH, S_IWOTH, S_IXOTH};
    
    struct stat statbuf;
    unsigned int binNum = 0;
    
    if (lstat(dir_str, &statbuf) < 0) {
        fprintf(stderr, "Error: Cannot stat '%s'. %s.\n", dir_str,
                strerror(errno));
        return (EXIT_FAILURE);
    }
    //printf("%d\n", statbuf.st_mode);
    int x = 0;
    for (int i = 0; i < 9; i += 3) {
        //permission_valid = statbuf.st_mode & perms[i];
        //printf("%d\n", permission_valid);
        //if (permission_valid) {
            if (*(perm_str+i) == 'r'){
                binNum = perms[i] | binNum;
                //continue;
            } else if(*(perm_str+i) == '-'){
                x++;
            }
            else{
                fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", perm_str);
                return EXIT_FAILURE;
            }
        //}
        //permission_valid = statbuf.st_mode & perms[i+1];
        //if (permission_valid) {
            if (*(perm_str+i+1) == 'w'){
                binNum = perms[i+1] | binNum;
                //continue;
            } else if(*(perm_str+i+1) == '-'){
                x++;
            }
            else{
                fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", perm_str);
                return EXIT_FAILURE;
            }
        //}
        //permission_valid = statbuf.st_mode & perms[i+2];
        //if (permission_valid) {
            if (*(perm_str+i+2) == 'x'){
                binNum = perms[i+2] | binNum;
                //continue;
            } else if(*(perm_str+i+2) == '-'){
                x++;
            }
            else{
                fprintf(stderr, "Error: Permissions string '%s' is invalid.\n", perm_str);
                return EXIT_FAILURE;
            }
        //}
        //printf("%d\n", binNum);
    }
    
    
    char *mypath = ((char *)malloc((PATH_MAX) * sizeof(char)));
    if(realpath(dir_str, mypath) == NULL){
        //printf("After realpath is called: %s\n", mypath);
        fprintf(stderr, "Error: Cannot get full path of file '%s'. %s\n", dir_str, strerror(errno));
        exit(EXIT_FAILURE);
    }
    //printf("Before recursive Call!\n");
    encFiles(mypath, binNum, &statbuf, 0, perm_str);
    free(dir_str);
    free(mypath);
     
        
    return EXIT_SUCCESS;
    
}
