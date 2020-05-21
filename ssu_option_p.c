#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

void print_usage();
void option_p(char* src) {
	struct stat stat_src;
	struct tm *atime;
	struct tm *mtime;
	struct tm *ctime;
	struct passwd *uid;
	struct group *gid;
	lstat(src, &stat_src); //파일의 정보를 stat 구조체에 저장
	atime = localtime(&stat_src.st_atime);
	mtime = localtime(&stat_src.st_mtime);
	ctime = localtime(&stat_src.st_ctime);
	uid = getpwuid(stat_src.st_uid);
	gid = getgrgid(stat_src.st_gid);
	printf("*********************file info*****************************\n");
	printf("파일 이름 : %s\n", src);
	printf("데이터의 마지막 읽은 시간 : %d.%d.%d %d:%d:%d\n", atime->tm_year+1900, atime->tm_mon+1, atime->tm_mday, atime->tm_hour, atime->tm_min, atime->tm_sec);
	printf("데이터의 마지막 수정 시간 : %d.%d.%d %d:%d:%d\n", mtime->tm_year+1900, mtime->tm_mon+1, mtime->tm_mday, mtime->tm_hour, mtime->tm_min, mtime->tm_sec );
	printf("데이터의 마지막 변경 시간 : %d.%d.%d %d:%d:%d\n", ctime->tm_year+1900, ctime->tm_mon+1, ctime->tm_mday, ctime->tm_hour, ctime->tm_min, ctime->tm_sec);
	printf("OWNER : %s\n", uid -> pw_name);
	printf("GROUP : %s\n", gid -> gr_name);
	printf("file size : %ld\n", stat_src.st_size);
}
