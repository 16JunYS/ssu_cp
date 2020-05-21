#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <utime.h>

void option_l(char *target, struct stat src_stat) {
	struct utimbuf time_buf;
		//change mode
		if(chmod(target, src_stat.st_mode) < 0) {
			fprintf(stderr, "chmod : error\n");
			exit(1);
		}

		//change utime
		time_buf.actime = src_stat.st_atime;
		time_buf.modtime = src_stat.st_mtime;
		if(utime(target, &time_buf) < 0) {
			fprintf(stderr, "utime error\n");
			exit(1);
		}

		//change pid, gid
		if(chown(target, src_stat.st_uid, src_stat.st_gid) < 0) {
			fprintf(stderr, "chown error\n");
			exit(1);
		}
}
