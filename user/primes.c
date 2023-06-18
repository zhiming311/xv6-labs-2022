// simple primes

#include <stddef.h>

#include "kernel/types.h"
#include "user/user.h"

static int *
get_pipe()
{
    int *local_pipe = (int *)malloc(2 * sizeof(int));
    pipe(local_pipe);
    return local_pipe;
}

static void
free_pipe(int *pipe)
{
    free((void *)pipe);
}

static int
on_eof(int byte, int *pipe_child)
{
    int ret = -1;
    if (byte == 0) {
        if (pipe_child != NULL) {
            close(pipe_child[1]);
            wait(0);
        }
        ret = 0;
    }
    return ret;
}

static int
process(int *pipe_init)
{
    int _num = 0;
    int num_read = 0;
    int pid_child = 0;
    int ret = 0;
    int *pipe_parent = pipe_init;
    int *pipe_child = NULL;
    
    close(0);
    close(pipe_parent[1]);
    while(1){
        ret = read(pipe_parent[0], &num_read, sizeof(int));
        if (on_eof(ret, pipe_child) == 0) {
            break;
        }
        if (_num == 0) {
            _num = num_read;
            printf("prime %d\n", _num);
        }
        if(num_read % _num == 0){
            continue;
        }
        if (pipe_child == NULL) {
            pipe_child = get_pipe();
            pid_child = fork();
        }
        if (pid_child == 0) {
            _num = 0;
            pipe_parent = pipe_child;
            pipe_child = NULL;
            close(pipe_parent[1]);
        } else {
            close(pipe_child[0]);
            write(pipe_child[1], &num_read, sizeof(int));
        }
    }
    if (pipe_child != NULL) {
        free_pipe(pipe_child);
    }
    exit(0);
}

int
main(int argc, char *argv[])
{
    int *pipe_init = get_pipe();

    for (int i = 2; i < 35; i++) {
        write(pipe_init[1], &i, sizeof(int));
    }
    close(pipe_init[1]);
    process(pipe_init);

    free_pipe(pipe_init);
    exit(0);
}
