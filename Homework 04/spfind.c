/*******************************************************************************
* Name        : spfind.c
* Author      : Avaneesh Kolluri
* Date        : 3/24/2020
* Description : HW4
* Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
******************************************************************************/
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <wait.h>




bool starts_with(const char *str, const char *prefix) {
    if(strlen(prefix) > strlen(str)){
        return false;
    }
    
    for(int i = 0; i<strlen(prefix); i++){
        if(prefix[i] != str[i]){
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[]) {
	int pfind_to_sort[2], sort_to_parent[2];

	if (pipe(pfind_to_sort) == -1){
    	fprintf(stderr, "pfind to sort pipe failed: %s\n",strerror(errno));
    	return EXIT_FAILURE;
    	}
   
    if(pipe(sort_to_parent) == -1){
    	fprintf(stderr, "sort to parent pipe failed: %s\n",strerror(errno));
    	return EXIT_FAILURE;
    }


    pid_t pid[2]; 


    if ((pid[0] = fork()) < 0){
    	return EXIT_FAILURE;
    }
    else if (pid[0] == 0){
    	//PFIND PROCESS
    	if (close(sort_to_parent[0]) == -1){
    		fprintf(stderr, "sort to parent pipe failed to close: %s\n",strerror(errno));
    		return EXIT_FAILURE;
    	}
    	if (close(sort_to_parent[1]) == -1){
    		fprintf(stderr, "sort to parent pipe failed to close: %s\n",strerror(errno));
    		return EXIT_FAILURE;
    	}


    	if (close(pfind_to_sort[0]) == -1){
    		fprintf(stderr, "pfind to sort pipe failed to close: %s\n",strerror(errno));
    		return EXIT_FAILURE;
    	}
    	if (dup2(pfind_to_sort[1],STDOUT_FILENO) == -1){
            close(pfind_to_sort[1]);
    		fprintf(stderr, "pfind to sort pipe failed to duplicate: %s\n",strerror(errno));
    		return EXIT_FAILURE;
    	}

        //execlp("pfind", "pfind", argv[1], argv[2], NULL);
        if (execv("pfind", argv) < 0){
            fprintf(stderr, "Error: pfind failed.\n");
            return EXIT_FAILURE;
        }
    }

   
  
    if ((pid[1] = fork()) < 0){
    	return EXIT_FAILURE;
	}
    else if (pid[1] == 0){
  		//SORT PROCESS  

    	if (close(pfind_to_sort[1]) == -1){
    		fprintf(stderr, "pfind to sort pipe failed to close: %s\n",strerror(errno));
    		return EXIT_FAILURE;
    	}
    	if (dup2(pfind_to_sort[0],STDIN_FILENO) == -1){
            close(pfind_to_sort[0]);
    		fprintf(stderr, "pfind to sort pipe failed to duplicate: %s\n",strerror(errno));
    		return EXIT_FAILURE;
    	}


    	if (close(sort_to_parent[0]) == -1){
    		fprintf(stderr, "sort to parent pipe failed to close: %s\n",strerror(errno));
    		return EXIT_FAILURE;
    	}
    	if (dup2(sort_to_parent[1],STDOUT_FILENO)== -1){
            close(sort_to_parent[1]);
    		fprintf(stderr, "sort to parent pipe failed to duplicate: %s\n",strerror(errno));
    		return EXIT_FAILURE;
    	}

    	//execlp
        if(execlp("sort", "sort", NULL) < 0){
            fprintf(stderr, "Error: sort failed.\n");
            return EXIT_FAILURE;
        }

    }

    //PARENT PROCESS

    if (close(pfind_to_sort[0]) == -1)
    	{
    	fprintf(stderr, "pfind to sort pipe failed to close: %s\n",strerror(errno));
    	return EXIT_FAILURE;
    	} //close pfind 1 to parent 0
    if (close(pfind_to_sort[1]) == -1)
    	{
    	fprintf(stderr, "pfind to sort pipe failed to close: %s\n",strerror(errno));
    	return EXIT_FAILURE;
    	} 

    if (close(sort_to_parent[1]) == -1){
    	fprintf(stderr, "sort to parent pipe failed to close :%s\n",strerror(errno));
    	return EXIT_FAILURE;
    	}
    if (dup2(sort_to_parent[0],STDIN_FILENO) == -1){
        close(sort_to_parent[0]);
    	fprintf(stderr, "sort to parent pipe failed to duplicate :%s\n",strerror(errno));
    	return EXIT_FAILURE;
    	}
    
    
    //While loop to read and wait statements read info
    
    char buffer[8192];
    int numct = 0; // this will be my number of lines
    ssize_t bytes_read;
    char usage[8192];
    
    
    while ((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1))) {//is it buffer -1?
        if (bytes_read == -1){
            if (errno == EINTR){
                continue;
            } else{
                perror("read() has failed.");
                exit(EXIT_FAILURE);
            }
        }
        
        for(int i=0;i<bytes_read;i++){
            if(write(STDOUT_FILENO, &buffer[i], 1) < 0){
                perror("write() has failed.");
                exit(EXIT_FAILURE);
            }
            //printf("%d\n",numct);
            if(numct == 0){
                //printf("%s\n",buffer);
                usage[i] = buffer[i];
            }
            if((buffer[i]) == '\n'){
                numct++;
            }
        }
    }
    
    
    
    int status;
    bool err = true;
    if(waitpid(pid[0],&status,0) < 0){
        fprintf(stderr, "First wait failed: %s\n",strerror(errno));
        return EXIT_FAILURE;
    }
    
    if (!WIFEXITED(status)) {
        fprintf(stderr, "WIFEXITED failed: %s\n",strerror(errno));
        return EXIT_FAILURE;
    }
    
    if(WEXITSTATUS(status) != EXIT_SUCCESS){
        err = false;
    }
    
    if(waitpid(pid[1],&status,0) < 0){
        fprintf(stderr, "Second wait failed: %s\n",strerror(errno));
        return EXIT_FAILURE;
    }
    
    if (!WIFEXITED(status)) {
        fprintf(stderr, "WIFEXITED failed: %s\n",strerror(errno));
        return EXIT_FAILURE;
    }
    
    if(WEXITSTATUS(status) != EXIT_SUCCESS){
        err = false;
    }

    if(!starts_with(usage, "Usage:") && err){
        printf("Total matches: %d\n", numct);
    }
    return EXIT_SUCCESS;
}

