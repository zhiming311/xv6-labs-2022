// Assignment 1, ping-pong

#include "kernel/types.h"
#include "user/user.h"

void
child_process(char key, int *in, int *out)
{
    char buffer;
    while(1){
        read(*in, &buffer, 1);
	if(buffer == key){
	    break;
	}
    }
    printf("%d: received ping\n", getpid());
    write(*out, &key, 1);
}

void
parent_process(char key, int *in, int *out)
{
    char buffer;
    write(*out, &key, 1);
    while(1){
	read(*in, &buffer, 1);
        if(buffer == key){
	    break;
	}
    }
    printf("%d: received pong\n", getpid());
}

int
main(int argc, char *argv[])
{
    int parent_pipe[2];
    int child_pipe[2];
    char key = 'a';

    pipe(parent_pipe);
    pipe(child_pipe);

    int pid = fork();
    if(pid == 0){
        child_process(key, &(parent_pipe[0]), &(child_pipe[1]));
    } else if(pid > 0){
        parent_process(key, &(child_pipe[0]), &(parent_pipe[1]));
    } else {
        printf("fork error\n");
    }
    exit(0);
}
