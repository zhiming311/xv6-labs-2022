// Simple sleep implementation

#include "user.h"

int main(int argc, char *argv[])
{
	int	ticks;
	int ret;

	if(argc <= 1){
		fprintf(2, "usage: sleep [ticks]\n");
		exit(1);
	}

	ticks = atoi(argv[1]);
	ret = sleep(ticks);

	exit(ret);
}
