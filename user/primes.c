#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include <stddef.h>
#include "user/user.h"

#define MAX_LEN 36

void sieve(int pipe_read){
    int fd[2];
    pipe(fd);
    int r_size=0,r_res=0, first=0;
    r_size = read(pipe_read, &r_res, sizeof(int));
    if(r_size > 0){
        first = r_res;
        printf("prime:%d \n", first);
    }else{
        exit(0);
    }

    int pid = fork();
    if(pid==0){
        close(fd[1]);
        sieve(fd[0]);
    }else{
        close(fd[0]);
        while(r_size >0){
            if(r_res % first){
                write(fd[1], &r_res, sizeof(int));
            }
            r_size = read(pipe_read, &r_res, sizeof(int));
        }
        close(pipe_read);
        close(fd[1]);
    }
}

int main(int argc, char *argv[]){
    if(argc != 1){
        printf("primes: need no parameter \n");
    }

    int fd[2];
    pipe(fd);
    int pid = fork();
    if(pid == 0){
        close(fd[1]);
        sieve(fd[0]);
    }else{
        close(fd[0]);
        printf("prime: 2\n");
        for(int i = 2; i < MAX_LEN; i++){
            if(i % 2){
                write(fd[1], &i, sizeof(int));
            }
        }
        close(fd[1]);
    }
    wait(NULL);
    exit(0);
}