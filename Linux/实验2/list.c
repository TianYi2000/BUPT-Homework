//#include <unistd.h>

#include <time.h>
#include <errno.h>
#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#include <sys/stat.h>
#include <sys/types.h>
#include "my_getopt.h"


//#define DEBUG

//定义选项序号
#define OPT_A 0
#define OPT_R 1
#define OPT_L 2
#define OPT_H 3
#define OPT_M 4
#define NO_PATH 5
#define OPT_MAXSIZE 6

#define FNAME_MAXLEN 600

//定义选项状态结构体列表
struct OPTION
{
    int flag,    //是否有该选项
        val;    //该选项的参数值
}optlist[OPT_MAXSIZE];

//定义给用户的帮助信息，提示此命令的用法
const char* HELP_MSG = 
"Usage: list1 [OPTION]... [FILE]...\n\
List information about the FILEs (the current directory by default)\n\
\n\
- a           Do not hide entries starting with .\n\
- r           List subdirectories recursively\n\
- l <bytes>   Minimum of file size\n\
- h <bytes>   Maximum of file size\n\
- m <days>    Limit file last modified time\n";

void pathadd(const char* a, const char* b, char* c) {
    memset(c, 0, sizeof(c));
    strcat(c, a);
    strcat(c, b);
    strcat(c, "/");
    return;
}

void list(const char* prefix, const char* fname);
void list_dir(const char* prefix, const char* fname);
void list_reg(const char* prefix, const char* fname);

//列出目录文件
void list_dir(const char* prefix, const char* fname) {
#ifdef DEBUG
    printf("DEBUG: list_dir: prefix=%s, fname=%s\n", prefix, fname);
#endif // DEBUG

    //打开目录
    DIR* dir;
    if ((dir = opendir(fname)) == NULL) {
        printf( "List: Can't access dir \"%s%s\": %s\n", prefix,fname, strerror(errno));
        return;
    }

    //进入目录
    chdir(fname);
    struct dirent* entry;   //读取目录表项
    char* new_prefix;
    //未输入地址时的特殊处理
    if (optlist[NO_PATH].flag && strcmp(fname, ".") == 0) {
        new_prefix = "";
    }
    else {
        new_prefix = (char*)malloc(strlen(prefix) + strlen(fname) + 3);   //生成新地址
        pathadd(prefix, fname, new_prefix);
    }
    
    //有递归选项
    if (optlist[OPT_R].flag) {
        rewinddir(dir);      //更新工作目录
        while ((entry = readdir(dir)) != NULL) {
            const char* entry_name = entry->d_name;
            if (!optlist[OPT_A].flag && entry_name[0] == '.')
                continue;
            if (strcmp(entry_name, ".") == 0 || strcmp(entry_name, "..") == 0)
                continue;
            list(new_prefix, entry_name);
        }
    }
    //无递归选项
    else {     
        while ((entry = readdir(dir)) != NULL) {
            const char* entry_name = entry->d_name;
            if (!optlist[OPT_A].flag && entry_name[0] == '.')
                continue;
            if (strcmp(entry_name, ".") == 0 || strcmp(entry_name, "..") == 0)
                continue;
            list_reg(new_prefix, entry_name);
        }
    }
    if (closedir(dir)) {
        printf("List: Can't close dir \"%s%s\": %s\n", prefix, fname, strerror(errno));
        return;
    }
    chdir("..");    //回溯
//   if (strcmp(new_prefix, ""))
//        free(new_prefix);
    return;
}

//列出磁盘文件
void list_reg(const char* prefix, const char* fname) {
#ifdef DEBUG
    printf("DEBUG: list_reg: prefix=%s, fname=%s\n", prefix, fname);
#endif // DEBUG

    //获取路径名path对应的i节点中的属性
    struct stat st;
    time_t nowtime;
    time(&nowtime);
    if (stat(fname, &st)) {
        printf("List: Can't access \"%s\": %s\n", fname, strerror(errno));
        return;
    }
    off_t fsize = st.st_size;
    time_t fmtime = st.st_mtime;
    if (optlist[OPT_L].flag && fsize < optlist[OPT_L].val ||
        optlist[OPT_H].flag && fsize > optlist[OPT_H].val ||
        optlist[OPT_M].flag && nowtime - fmtime > optlist[OPT_M].val * 24 * 60 * 60)
        return;
    printf("%s\t%10ld\t%s%s\n", (S_ISDIR(st.st_mode) ? "DIR" : "FILE"), fsize, prefix, fname);
    return;
}

//list操作的主函数，输入文件或目录名，输出根据选项处理后文件列表
void list(const char* prefix, const char* fname) {
#ifdef DEBUG
    printf("DEBUG: list: prefix=%s, fname=%s\n", prefix, fname);
#endif // DEBUG


    //获取路径名path对应的i节点中的属性
    struct stat st; 
    if (stat(fname, &st)) {
        printf("List: Can't access \"%s\": %s\n", fname, strerror(errno));
        return;
    }

    //判断是否为目录
    if (S_ISDIR(st.st_mode)) {
        list_dir(prefix, fname);
    }
    //普通磁盘文件
    else if (S_ISREG(st.st_mode)) {
        list_reg(prefix, fname);
    }
    //非目录、也非磁盘文件
    else {
#ifdef DEBUG
        printf("DEBUG: %s 是一个非目录、也非磁盘文件\n", fname);
#endif // DEBUG
    }
    return;
}

int main(int argc, char** argv)
{
    
#ifdef DEBUG
    const int argc_const = argc;
    printf("argc = %d, &argc=%lld\n", argc, &argc);
    for (int i = 0; i < argc; ++i)
    {
        printf("argv[%d]=%s\n", i, argv[i]);
    }
#endif // DEBUG
 
    char c = 0;
    int cnt = 0;

    //处理输入的选项
    while ((c = my_getopt(argc, argv, "ral:h:m:")) != -1) {
#ifdef DEBUG
        printf("DEBUG: No.%d option is %c\n", ++cnt, c);
#endif // DEBUG
        switch (c)
        {
        case 'a':
            optlist[OPT_A].flag = 1;
            break;
        case 'r':
            optlist[OPT_R].flag = 1;
            break;
        case 'l':
            optlist[OPT_L].flag = 1;
            optlist[OPT_L].val = optarg;
            break;
        case 'h':
            optlist[OPT_H].flag = 1;
            optlist[OPT_H].val = optarg;
            break;
        case 'm':
            optlist[OPT_M].flag = 1;
            optlist[OPT_M].val = optarg;
            break;
        default:
            break;
        }
    }

    //选项处理过程中出现错误
    if (opterr) {
        char* errmsg = (char*)malloc(ERRMSG_MAXSIZE);
        memset(errmsg, 0, sizeof(errmsg));
        get_errmsg(errmsg, argv);
        printf("List: %s\n", errmsg);
        free(errmsg);
        printf("%s", HELP_MSG);
        return 0;
    }

    //完成选项的解析，准备处理
#ifdef DEBUG
    printf("DEBUG: 选项解析完毕，准备处理\n");
#endif // DEBUG
    //选项后无文件或目录名，则输出当前目录所有文件
    if (optind >= argc) {
        optlist[NO_PATH].flag = 1;
        list("", ".");  //默认输出当前目录所有文件
    }
    //选项后有文件或目录名，遍历之
    else
    {
        for (int i = optind; i < argc; ++i) {
            list("", argv[i]);
#ifdef DEBUG
            argc = argc_const;
#endif // DEBUG

        }
            
    }

    return 0;
}

