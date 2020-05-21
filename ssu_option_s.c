#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

void print_usage();
void option_s(char *src, char *tar) {
	struct stat stat_src;	

	stat(src, &stat_src); //source 파일 정보 구조체 stat_src에 저장

	if(S_ISDIR(stat_src.st_mode)); //source 파일이 directory인 경우 생략

	else {
		if(symlink(src, tar) < 0) {
			fprintf(stderr, "ssu_cp : target file already exists!\n");
			print_usage();
			exit(1);
		}	
	}
}
