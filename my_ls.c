/*************************************************************************
	> File Name: my_ls.c
	> Author: 
	> Mail: 
	> Created Time: 2018年09月22日 星期六 11时39分21秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>  //linux基本系统数据类型头文件
#include <dirent.h>
#include <sys/stat.h>   //linux系统定义文件状态所在的伪标准头文件
#include <pwd.h>        //提供对/etc/passwd文件访问的函数
#include <grp.h>
#include <unistd.h>  //提供通用的文件，目录，程序及进程操作的函数
#include <string.h>

void ls_al(char *dirname);
void dostat(char *filename);
void show_file_into(char *filename, struct stat *info_p);
void mode_to_letters(int mode, char *str);
char *uid_to_name(uid_t uid);
char *gid_to_name(gid_t gid);

void show_file_into(char *filename, struct stat *info_p) {

    char *uid_to_name(), *ctime(), *gid_to_name(), *filemode();
    void mode_to_letters();
    char modestr[11];

    mode_to_letters(info_p->st_mode, modestr);

    //printf(" %s", modestr);
    printf(" %4d", (int) info_p->st_nlink);
    printf(" %-8s", uid_to_name(info_p->st_uid));
    printf(" %-8s", gid_to_name(info_p->st_gid));
    printf(" %8ld", (long) info_p->st_size);
    printf(" %.12s", 4 + ctime(&info_p->st_mtime));
    if(modestr[0] == 'd') {
        printf(" \33[36m%s\n\33[0m ", filename);   
    } else if(modestr[3] == 'x' || modestr[6] == 'x' || modestr[9] == 'x') {
        printf(" \33[32m%s\n\33[0m", filename);
    } else {
        printf(" %s\n", filename);
    }

}

void mode_to_letters(int mode, char str[]) {

    strcpy(str, "----------");

    if (S_ISDIR(mode)) str[0] = 'd';  //目录
    if (S_ISCHR(mode)) str[0] = 'c';  //字符设备
    if (S_ISBLK(mode)) str[0] = 'b';  //设备块
    if ((mode & S_IRUSR)) str[1] = 'r'; 
    if ((mode & S_IWUSR)) str[2] = 'w';
    if ((mode & S_IXUSR)) str[3] = 'x';
    if ((mode & S_IRGRP)) str[4] = 'r'; 
    if ((mode & S_IWGRP)) str[5] = 'w';
    if ((mode & S_IXGRP)) str[6] = 'x';
    if ((mode & S_IROTH)) str[7] = 'r';  
    if ((mode & S_IWOTH)) str[8] = 'w';
    if ((mode & S_IXOTH)) str[9] = 'x';
    printf("%s", str);
}

/*char *uid_to_name(uid_t uid) {
    struct passwd *getpwuid(), *pw_ptr;
    static char numstr[10];

    if((pw_ptr = getpwuid(uid)) == NULL) {
        sprintf(numstr, "%d", uid);
        return numstr;
    } else {
        return pw_ptr->pw_name;
    }
}*/

char *uid_to_name(uid_t uid) {
    struct passwd *pw_ptr;
    pw_ptr = getpwuid(uid);
    return pw_ptr->pw_name;
}

/*char *gid_to_name(gid_t gid) {
    struct group *getgrgid(), *grp_ptr;
    static char numstr[10];

    if((grp_ptr = getgrgid(gid)) == NULL) {
        sprintf(numstr, "%d", gid);
        return numstr;
    } else {
        return grp_ptr->gr_name;
    }
}*/

char *gid_to_name(gid_t gid) {
    struct group *grp_ptr;
    grp_ptr = getgrgid(gid);
    return grp_ptr->gr_name; 
}

void ls_al(char dirname[]) {
    DIR *dir_ptr;
    struct dirent *direntp;

    if((dir_ptr = opendir(dirname)) == NULL) {
        fprintf(stderr, "ls : cannot open %s \n", dirname);
        dostat(dirname);
    } else {
        while((direntp = readdir(dir_ptr)) != NULL) {
            dostat(direntp->d_name);
        }
        closedir(dir_ptr);
    }
}

void dostat(char *filename) {
    struct stat info;
    if(stat(filename, &info) == -1) {
        perror(filename);
    } else {
        show_file_into(filename, &info);
    }
}

int main(int ac, char *av[]) {
    if(ac == 1) {
        ls_al(".");
    } 
    else {
        while(--ac) {
            printf("%s: \n",++*av);
            ls_al(*av);
        }
    }
}
