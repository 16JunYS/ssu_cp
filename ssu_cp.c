#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>

#define FILE_LENGTH 256
#define BUFFER_SIZE	1024
typedef int OPTION;
OPTION on = 0;
OPTION S = 0;
OPTION I = 0;
OPTION L = 0;
OPTION N = 0;
OPTION P = 0;
OPTION R = 0;
OPTION D = 0;

void option(int, char *argv[]); //process Option
void option_s(char* src, char* tar); //Option -s
void option_p(char* src); //Option -p
void option_r(char* src, char* tar); //Option -r
void option_l(char* tar, struct stat stat_src); //Option -l
void ssu_copy(char *, char *);
void print_usage();
int main(int argc, char *argv[])
{
	int fd;
	char *src;
	char *tar;
	char opt_i;
	char new_tar[FILE_LENGTH];
	struct stat src_stat;
	if (argc < 3) {
		print_usage();
		exit(1);
	}
	
	tar = argv[argc-1];
	src = argv[argc-2];
	if(strcmp(tar, src) == 0) {
		fprintf(stderr, "ssu_cp : %s: source file and directory cannot be the same\n", src);
		print_usage();
		exit(1);
	}

	chdir(".");
	printf("target : %s\n", tar);
	printf("src : %s\n", src);
	
	option(argc, argv);
	if(R) {
		if(mkdir(tar, 0766) < 0) { // 이미 존재하는 같은 이름을 가진  디렉터리를 만들려고 할 때 에러 발생-> 별도의 옵션이 없을 경우 덮어쓰기 진행
			if(I) { //사용자에게 덮어쓰기 여부를 물어보는 옵션 -i
				printf("overwrite directory %s (y/n)?", tar);
				scanf("%c", &opt_i);
				if(opt_i == 'y') N = 0;
				else N = 1;
			}
			if(N) {
				exit(0);
			}
			else {
				opendir(tar); //open [target] directory
			}
		}
		option_r(src, tar); //하위 디렉터리와 모든 파일들을 복사하는 함수
	}
	else {
		if(open(tar, O_RDWR | O_CREAT | O_EXCL, 0664) < 0) { 
			//이미 존재하는 [target] 파일인 경우
			if(I) {		
				printf("overwrite %s (y/n)? ", tar);
				scanf("%c", &opt_i);
				if(opt_i == 'y') {
					N = 0;
				}
				else N = 1;
			}
			if(N) exit(0);
		}
		ssu_copy(src, tar);
	}
	//option 'l' is on
	if (L == 1) {
		stat(src, &src_stat);
		option_l(tar, src_stat);
	}
}

void option(int OptNum, char *Option[])
{
	int c;
	int repeat_option = 0; //옵션 중복 처리
	char *src = Option[OptNum-2];
	char *tar = Option[OptNum-1];
	char opt_i;
	int optD_N;
	struct stat src_stat;
	int error_d = 0;

	stat(src, &src_stat);
	
	while((c = getopt(OptNum, Option, "sSiIlLnNpPrRdD:")) != -1) {
		switch(c) {
			case 's' : //symoblic link 생성
			case 'S':
				S++;
				on++;
				if(OptNum != 4) {
					fprintf(stderr, "ssu_cp: -s\n");
					print_usage();
					exit(1);
				}
					printf("s option is on\n");
					option_s(src, tar);
					exit(0);
				return;
			case 'i' : //강제로 덮어쓰기
			case 'I' :
				I++;
				on++;
				if(S == 1) {
					fprintf(stderr, "ssu_cp : cannot use option %c with option s\n", c);
					print_usage();
					exit(1);
				}
				if(I == 1) {
					printf("i option is on\n");
				}
				else repeat_option = 1;
				break;
			case 'l' : //파일 정보 [TARGET]에 복사
			case 'L' :
				L++;
				on++;
				if(S == 1) {
					fprintf(stderr, "ssu_cp : cannot use option %c with option s\n", c);
					print_usage();
					exit(1);
				}
				if(L == 1) {
					printf("l option is on\n");
					
				}
				else repeat_option = 1;
				break;
			case 'n' : //기존 파일이 있는 경우 덮어쓰지 않음
			case 'N' :
				N++;
				on++;
				if(S == 1) {
					fprintf(stderr, "ssu_cp : cannot use option %c with option s\n", c);
					print_usage();
					exit(1);
				}
				if (N == 1) {
					printf("n option is on\n");
				}
				else repeat_option = 1;
				break;
			case 'p' : //파일 정보 출력 후 cp 수행
			case 'P' :
				P++;
				on++;
			//	printf("ON : %d\n", on);
				if(S == 1) {
					fprintf(stderr, "ssu_cp : cannot use option %c with option s\n", c);
					print_usage();
					exit(1);
				}
				if (P == 1) {
					printf("p option is on\n");
					option_p(src); //파일의 정보를 출력하는 함수 호출
				}
				else repeat_option = 1;
				break;
			case 'r' :
			case 'R' :
				R++;
				on++;
				//-s 또는 -d 옵션이 중복된 경우, [SOURCE]파일이 디렉터리가 아닌경우 에러처리
				if(S == 1 || D == 1 || !S_ISDIR(src_stat.st_mode)) { 
					if(S)
						fprintf(stderr, "ssu_cp : cannot use option %c with option s\n", c);
					if(D)
						fprintf(stderr, "ssu_cp : cannot use option %c with option d\n", c);
					if(!S_ISDIR(src_stat.st_mode))
						fprintf(stderr, "ssu_cp : source file must be directory with option %c\n", c);
					print_usage();
					exit(1);
				}
				if (R == 1) {
					printf("r option is on\n");
					
				}
				else repeat_option = 1;
				break;

			case 'd' :
			case 'D' :
				D++;
				on++;
				//-s 또는 -r 옵션이 중복된 경우, [SOURCE]파일이 디렉터리가 아닌경우 에러처리
				if(S == 1 || R == 1) {
					if(S)
						fprintf(stderr, "ssu_cp : cannot use option %c with option s\n", c);
					if(R)
						fprintf(stderr, "sus_cp : cannot use option %c with option r\n", c);
					
					if(optarg == NULL) {
						fprintf(stderr, "ssu_cp : -d [NUMBER]\n");
					}
					if(!S_ISDIR(src_stat.st_mode))
						fprintf(stderr, "ssu_cp : source file must be directory with option %c\n", c);
					print_usage();
					exit(1);
				}
				if (D == 1) {
					//if(optarg == NULL) {
					//	fprintf(stderr, "ssu_cp : -d [NUMBER] \n");
					//	error_d = 1;
					//}
					if(!S_ISDIR(src_stat.st_mode)) {
						fprintf(stderr, "ssu_cp : source file must be directory with option %c\n", c);
						error_d = 1;
					}
					if (error_d) {
						print_usage();
						exit(1);
					}
					printf("d option is on\n");
					optD_N = atoi(optarg);
					printf("N : %d\n", optD_N);
				}
				else repeat_option = 1;
				break;
		}
		if (repeat_option) {
			printf("OPTION is repeated\n");
			print_usage();
			exit(0);
		}
	}
}

void ssu_copy(char *source, char *target) {
	struct stat src_stat;
	char opt_i;
	int sym = 0;
	int fd_src, fd_tar; //source file descriptor, target file descriptor
	int length;
	char c;
	int i, a;

	//source파일 정보 src_stat stat구조체에 저장
	stat(source, &src_stat);
	
	//source 파일이 directory인 경우 
	if(S_ISDIR(src_stat.st_mode) && (!R || !D)) { 
		printf("ssu_cp : source file cannot be directory file\n"); print_usage(); exit(1);
		print_usage();
		exit(1);
	}

	if ((fd_src = open(source, O_RDONLY)) < 0) { //읽기 권한으로 [SOURCE]파일 open
		fprintf(stderr, "ssu_cp : %s: No such file or directory\n", source);
		exit(1);
	}
	
	if((fd_tar = open(target, O_RDWR | O_CREAT | O_TRUNC, 0664)) < 0) { //[target]파일 생성(또는 덮어쓰기)
		fprintf(stderr, "target %s open error\n", target);
	}
	
	//source 파일을 target 파일로 복사
	while(read(fd_src, &c, 1) > 0) {
		write(fd_tar, &c, 1);
	}
}

void print_usage() {
	fprintf(stderr, "ssu_cp error\n");
	fprintf(stderr, "usage : in case of file\n");
	fprintf(stderr, "	cp [-i/n][-l][-p]	[source][target]\n");
	fprintf(stderr, "	or cp [-s][source][target]\n");
	fprintf(stderr, "	in case of directory cp [-i/n][-l][-p][-r][-d][N]\n");
}
