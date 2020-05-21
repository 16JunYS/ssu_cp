#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#define FILE_LENGTH 256

char TARGET[FILE_LENGTH];

void print_usage();
void option_s(char *src, char *tar);
void ssu_copy(char *src, char *tar);
void option_r(char *src, char *tar) {
	struct stat stat_src; //source 파일의 정보가 담긴 구조체
	struct dirent *dent_src; //source directory 정보가 담긴 구조체
	DIR *dp_src; //source 디렉터리 가리키는 포인터
	char fname_src[FILE_LENGTH] = {'\0'}; //source 파일의 하위 파일들의 경로 이름 저장
	char fname_tar[FILE_LENGTH] = {'\0'}; //source 파일의 하위 파일들의 경로 target 파일에 저장
	char buf[FILE_LENGTH] = {'\0'};
	int len_src, len_tar;
	int fd_src, fd_tar; //source 파일과 target 파일의 파일 디스크립터
	int i;
	char c;

	dp_src = opendir(src);
	
	while(1) {
		//src 디렉터리에 하위 파일이 있는 경우
		if((dent_src = readdir(dp_src)) != NULL) {
			if(dent_src->d_ino == 0) {
				continue;
			}
			//src 디렉터리의 하위 파일 경로 이름 저장
			len_src = strlen(src);
			strcpy(fname_src, src);
			fname_src[len_src] = '/';
			fname_src[len_src+1] = '\0';
			strcat(fname_src, dent_src->d_name);
			
			//tar 디렉터리에 src 하위 파일 경로 이름 저장
			strcpy(fname_tar, tar);
			len_tar = strlen(tar);
			fname_tar[len_tar] = '/';
			fname_tar[len_tar+1] = '\0';
			strcat(fname_tar, dent_src->d_name);
			if(stat(fname_src, &stat_src) < 0) {
				continue;
			}
			if(S_ISDIR(stat_src.st_mode)) { //하위 파일이 디렉터리인 경우
				if(strcmp(".", dent_src->d_name) == 0) { // '.' directory
					continue;
				}
				if(strcmp("..", dent_src->d_name) == 0) { //'..' directory
					continue;
				}
				mkdir(fname_tar, 0766); //tar 디렉터리에 src의 하위 디렉터리 생성
				option_r(fname_src, fname_tar);
			}
			else if (S_ISREG(stat_src.st_mode)) { //하위 파일이 일반 파일인 경우
				fd_src = open(fname_src, O_RDONLY);	
				
				ssu_copy(fname_src, fname_tar);
			}

			else;

			for (i = 0; i < FILE_LENGTH; i++) {
				fname_tar[i] = '\0';
				fname_src[i] = '\0';
				buf[i] = '\0';
			}
		}
		//src 디렉터리의 끝
		else {
			break;
		}
	}

	return;
}
