/*******************************************************************************
 * Name        : phelp.c
 * Author      :  Avaneesh Kolluri
 * Date        :  2/24/2020
 * Description : pfin's helper implementation.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <errno.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include<errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include<unistd.h>


char* permission_string(struct stat *statbuf, int perms[]) {
    
    char * str = (char *)malloc((10 + 1) * sizeof(char));
    int permission_valid;
    
    str[0] = '-';
    for (int i = 1; i < 10; i += 3) {
        permission_valid = statbuf->st_mode & perms[i - 1];
        if (permission_valid) {
            *(str+i) = 'r';
            //printf("r");
        } else{
            //printf("-");
            *(str+i) = '-';
        }
        permission_valid = statbuf->st_mode & perms[i];
        if (permission_valid) {
            //printf("w");
            *(str+i+1) = 'w';
        } else{
            //printf("-");
            *(str+i+1) = '-';
        }
        permission_valid = statbuf->st_mode & perms[i+1];
        if(permission_valid){
            //printf("x");
            *(str+i+2) = 'x';
        } else{
            //printf("-");
            *(str+i+2) = '-';
        }
    }
    return str;
}

void encFiles(char *path, unsigned int permCode, struct stat *statbuf, int dirLength , char *perm_str){
    
    DIR *dp;
    struct dirent *dent;
    char filename[PATH_MAX];
    size_t plen;
    //struct stat statbuf;
    //unsigned int binNum = 0;
    //int permission_valid;
    int perms[] = {S_IRUSR, S_IWUSR, S_IXUSR,
    S_IRGRP, S_IWGRP, S_IXGRP,
    S_IROTH, S_IWOTH, S_IXOTH};
    
    
    //printf("Current Path before dp: %s\n", path);
    
    if ((dp = opendir(path)) == NULL){
        
        fprintf(stderr, "Error: Cannot open directory '%s'. %s.\n", path,strerror(errno));
        //Need to stop only this current branch of recursion
        //exit(EXIT_FAILURE);
        return;
    }
    //printf("Current Path after dp: %s\n", path);
    filename[0] = '\0';
    if (strcmp(path, "/")) {
      strncpy(filename, path, PATH_MAX);
    }
    //printf("Current filename before dp: %s\n", filename);
    
    errno = 0;
    
    plen = strlen(filename) +1;
    filename[plen-1] ='/';
    filename[plen] = '\0';
    
    
    while ((dent = readdir(dp)) != NULL) {
        //bool b = true;
        
        if(strcmp(dent->d_name,".") != 0 && strcmp(dent->d_name,"..") != 0){
            //printf("%s\n",dent->d_name);
            
        
    /* get information on file so we can tell if it is a directory or a non-directory */
        
        
        //printf("Current Path's Permission String: cd ..%s\n",permission_string(statbuf, perms));
    
    /* check if the directory entry is a directory itself */
        if (dent->d_type == DT_DIR) {
            
            //recursively call encFiles with the path of the directory being path
            
            int newplen = strlen(path) + 1;
            filename[newplen-1] ='/';
            filename[newplen] = '\0';
            

            strncpy(filename + newplen, dent->d_name, PATH_MAX - newplen);
            //save printf("Current Filename Path: %s\n", filename);
            //save printf("Current Filename: %s\n", dent->d_name);
            
            /*if(dirLength == 0){
                dirLength = strlen(path) + 1;
            }
            path[dirLength - 1] = '/';
            path[dirLength] = '\0';
            strncpy(path + dirLength, dent->d_name, PATH_MAX - dirLength);
            int dirLen = strlen(path);*/
            if (lstat(filename, statbuf) < 0) {
                fprintf(stderr, "Error: Cannot stat file '%s'. %s.\n", dent->d_name, strerror(errno));
                continue;
            }
            unsigned int buf = statbuf->st_mode;
            bool boolean = true;
            //printf("PERMISSION STRING OF %s: %s. With length of: %ld\n", filename, permission_string(statbuf, perms), strlen(permission_string(statbuf, perms)));
            /*
            
            char *temp = ((char *)malloc((strlen(permission_string(statbuf, perms))) * sizeof(char)));
            temp = permission_string(statbuf, perms);
            temp[10] = '\0';
            boolean = false;
            char temp2[9];
            for(int i = 0; i<9; i++){
                temp2[i] = temp[i+1];
            }
            free(temp);
            if (strcmp(temp2, perm_str) != 0){
                boolean = false;
            }
            */
            
            
            for(int i =0; i<9; i++){
                if ((permCode & perms[i])){
                    //printf("the if case: %d\n", (perms[i]));
                    if(!(buf & perms[i])){
                        //printf("broke false\n");
                        boolean = false;
                        break;
                    }
                } else {
                    
                    if((buf & perms[i])){
                        //printf("broke false\n");
                        boolean = false;
                        break;
                    }
                }
            }
            if(boolean){
                //printf("works\n");
                char *mypath = ((char *)malloc((PATH_MAX) * sizeof(char)));
                
                for(int i = 0; i < strlen(filename); i++){
                    *(mypath+i) = *(filename+i);
                }
                
                printf("%s\n",filename);
                free(mypath);
            }
            int dirLen = 0;
            dirLen++;
            
            encFiles(filename, permCode, statbuf, dirLen, perm_str);
            
        } else{
            //strcat(path,"/");
            //strncpy(path + dirLength + 1, dent->d_name, PATH_MAX - dirLength);
            
            strncpy(filename + plen, dent->d_name, PATH_MAX - plen);
            //printf("Current Filename TEXT Path: %s\n", filename);
            if (lstat(filename, statbuf) < 0) {
                fprintf(stderr, "Error: Cannot stat file '%s'. %s.\n", dent->d_name, strerror(errno));
                continue;
            }
            unsigned int buf = statbuf->st_mode;
            bool boolean = true;
            //printf("PERMISSION STRING OF %s: %s. With length of: %ld\n", filename, permission_string(statbuf, perms), strlen(permission_string(statbuf, perms)));
            /*
            
            char *temp = ((char *)malloc((strlen(permission_string(statbuf, perms))) * sizeof(char)));
            temp = permission_string(statbuf, perms);
            temp[10] = '\0';
            boolean = false;
            char temp2[9];
            for(int i = 0; i<9; i++){
                temp2[i] = temp[i+1];
            }
            free(temp);
            if (strcmp(temp2, perm_str) != 0){
                boolean = false;
            }
            */
            
            //printf("PERMISSION STRING OF %s: %s. With length of: %ld\n", filename, permission_string(statbuf, perms), strlen(permission_string(statbuf, perms)));
            for(int i =0; i<9; i++){
                if ((permCode & perms[i])){
                    //printf("the if case: %d\n", (perms[i]));
                    if(!(buf & perms[i])){
                        //printf("broke false\n");
                        boolean = false;
                        break;
                    }
                } else {
                    
                    if((buf & perms[i])){
                        //printf("broke false\n");
                        boolean = false;
                        break;
                    }
                }
            }
            
            if(boolean){
                //printf("works\n");
                
                printf("%s\n",filename);
            }
        }
        errno = 0;
        }
    }
    
    if (errno != 0) {
        fprintf(stderr, "Error: An error occured while reading the directory. %s.\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    closedir(dp);
}
