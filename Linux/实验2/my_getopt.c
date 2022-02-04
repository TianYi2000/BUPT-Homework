#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "my_getopt.h"


#ifdef DEBUG
void debug(char* const msg) {
    if ((strcmp(msg, "OPTIND") == 0))
        printf("DEBUG: OPTIND=%d\n", optind);
    else
        printf("DEBUG: %s\n", msg);
}
#endif // DEBUG

int optarg;
int optind = 1; 
int opterr = 0;

static char* optcursor = NULL;  //选项指针('-'后第一个字符的指针)

//将参数字符串转换为预期的数据类型(在本次编程作业中只有整数类型的参数)
int trans_int(char* const arg) {
    for (char* c = arg; *c != '\0'; c++) {
#ifdef DEBUG
        printf("DEBUG: 当前待识别是否为整数的参数字符为:%c\n", *c);
#endif // DEBUG

        if (*c < '0' || *c > '9')
            return -1;
    }
    return atoi(arg);
}


int my_getopt(int argc, char* const argv[], const char* optstring) {
    int optchar = EOF;  //选项字符，正确解析时为参数，错误时为'?'
    const char* optfind = NULL; //搜索选项模式得到的索引指针

        //判断当前是否处理完所有选项
    if (optind >= argc) {
#ifdef DEBUG
        debug("处理完所有选项,选项后无文件名或目录名");
#endif // DEBUG
        return optchar;
    }

#ifdef DEBUG
    debug("OPTIND");
#endif // DEBUG

    //初始情况，默认选项无参数
    optarg = -1;
    
    //选项前无'-'
    if (argv[optind][0] != '-') {
#ifdef DEBUG
        debug("选项前无-,这是一个文件名或目录名,参数处理结束");
#endif // DEBUG

        return optchar;
    }
        
    //选项丢失（选项只输了一个）
    if (strcmp(argv[optind], "-") == 0) {
#ifdef DEBUG
        debug("选项丢失（选项只输了一个）");
#endif // DEBUG

        opterr = REQUIRED_OPTION;
        return optchar;
    }

    //显式地终止命令选项分析
    if (strcmp(argv[optind], "--") == 0) {
#ifdef DEBUG
        debug("读取到--,显式地终止命令选项分析");
#endif // DEBUG

        optind++;
        return optchar;
    }

    //开始分析选项
    optcursor = argv[optind] + 1;   //选项第1位为'-'，第2位才为选项字符，因此加1

    //选项包含两个以上字符，由于本次编程作业不考虑多选项挤在一个命令行参数内的情况，故不合法
    if (optcursor[1] != '\0') {
#ifdef DEBUG
        debug("选项包含两个以上字符");
#endif // DEBUG
        opterr = INVALID_OPTION;
        return optchar = '?';
    }

    optchar = optcursor[0]; 
#ifdef DEBUG
    printf("DEBUG: 当前识别的选项为 %c\n", optchar);
#endif // DEBUG

    optfind = strchr(optstring, optchar);
    if (optfind) {
        if (optfind[1] == ':') { //当前搜索得到的模式含':'
             //当前选项需要的参数在下一个argv里,optind+1
            if (++optind < argc) { //如果还有下一个argv
#ifdef DEBUG
                printf("DEBUG: 当前识别参数为 %s\n", argv[optind]);
#endif // DEBUG
                if ((optarg = trans_int(argv[optind])) == -1) {
#ifdef DEBUG
                    printf("DEBUG: 当前参数'%s'不是一个整数\n", argv[optind]);
#endif // DEBUG
                    opterr = WRONG_TYPE_ARG;
                    return optchar = '?';
                }
#ifdef DEBUG
                printf("DEBUG: 当前参数'%s'是一个整数\n", argv[optind]);
#endif // DEBUG                   
            }
            else { //如果已经读完全部argv,说明最后一个需要选项的参数没有输入参数
#ifdef DEBUG
                debug("最后一个需要选项的参数没有输入参数");
#endif // DEBUG
                opterr = REQUIRED_ARG;
                return optchar = '?';
            }
            optcursor = NULL;   //当前选项处理完毕
        }
    }
    else {  //如果没有在给定选项列表中找到该选项，说明该选项不合法
#ifdef DEBUG
        debug("当前选项不合法");
#endif // DEBUG
        opterr = INVALID_OPTION;
        return optchar = '?';
    }

    ++optind;
    return optchar;
}


void get_errmsg(char *msg, char* const argv[])
{
    switch (opterr)
    {
    case INVALID_OPTION:    //无效选项
        strcat(msg, "invalid option '");
        strcat(msg, argv[optind]);
        strcat(msg, "'");
        break;
    case REQUIRED_OPTION:   //选项丢失
        strcat(msg, "please input option after '-'");
        break;
    case REQUIRED_ARG:      //参数缺失
        strcat(msg, "please input argument for '");
        strcat(msg, argv[optind - 1]);
        strcat(msg, "'");
        break;
    case WRONG_TYPE_ARG:    //参数类型错误
        strcat(msg, "please input correct argument for '");
        strcat(msg, argv[optind - 1]);
        strcat(msg, "'");
        break;
    default:
        strcat(msg, "unkhnown error");
    }
}