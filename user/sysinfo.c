#include "kernel/types.h"
#include "kernel/sysinfo.h"
#include "user/user.h"


int
main(int argc, char *argv[])
{
	struct sysinfo si;
	int ret = 0;

	if (sysinfo(&si) < 0 ) {
		printf("FAIL: sysinfo failed\n");
		ret = 1;
	}

	if (ret == 0) {
		printf("free memory:%d B\n", si.freemem);
		printf("process number:%d\n", si.nproc);
	}

	exit(ret);
}
