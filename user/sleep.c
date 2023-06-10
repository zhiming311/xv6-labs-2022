// Assignment 1. simple sleep

#include "kernel/types.h"
#include "user.h"

int main(int argc, char *argv[])
{
    int ticks = 0;
    int ret = 1;

    if((argc <= 1) || (argc > 2)){
        fprintf(2, "usage: sleep [ticks]\n");
        exit(1);
    }

    ticks = atoi(argv[1]);

    if(ticks <= 0){
        fprintf(2, "invalid ticks: %s\n", argv[1]);
        exit(1);
    }

    ret = sleep(ticks);
    exit(ret);
}
