// simpele find commnd

#include <stdbool.h>
#include <stddef.h>
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define MAX_PATH 512
#define MAX_NAME 64

static void format_path(char *path)
{
	// TODO: remove redundent "." and ".." in path.
}

static char *parse_filename(char *filepath)
{
	char *p = NULL;
	for(p = filepath + strlen(filepath); p >= filepath && *p != '/'; p--) {
		continue;
	}
	return ++p;
}

static int find(char *path, const char *target)
{
	int ret = 0;
	int fd = 0;
	char *filename = NULL;
	char *path_end = path + strlen(path);
	struct stat st;
	struct dirent de;

	fd = open(path, O_RDONLY);
	if (fd < 0) {
		fprintf(2, "find: cannot open %s\n", path);
		ret = 1;
	}
	if ((ret == 0) && (fstat(fd, &st) < 0)){
		fprintf(2, "find: cannot stat %s\n", path);
		ret = 1;
	}
	if (ret == 0) {
		switch(st.type){
		case T_DEVICE:
		case T_FILE:
			filename = parse_filename(path);
			if (strcmp(filename, target) == 0){
				printf("%s\n", path);
			}
			break;
		case T_DIR:
			memset(path_end++, '/', 1);
			while(read(fd, &de, sizeof(de)) == sizeof(de)) {
				if ((strcmp(de.name, ".") == 0) ||
					(strcmp(de.name, "..") == 0) ||
					(de.inum == 0)) {
					continue;
				}
				memcpy(path_end, de.name, strlen(de.name));
				find(path, target);
				memset(path_end, '\0', strlen(path_end));
			}
			memset(--path_end, '\0', 1);
			break;
		default:
			break;
		}
	}
	close(fd);
	return ret;
}

int main(int argc, char *argv[])
{
	int ret = 0;
	char buffer[MAX_PATH];
	char target[MAX_NAME];

	memset(buffer, '\0', MAX_PATH);
	memset(target, '\0', MAX_NAME);
	strcpy(buffer, argv[1]);
	strcpy(target, argv[2]);

	format_path(buffer);
	if (argc == 3) {
		ret = find(buffer, target);
	} else {
		printf("usage: find [directory] filename\n");
		ret = 1;
	}
	exit(ret);
}
