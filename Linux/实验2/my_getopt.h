#ifndef MY_GETOPT_H
#define MY_GETOPT_H

#define INVALID_OPTION 1	//无效选项；错误示例：list -z -> List: invalid option '-z'
#define REQUIRED_OPTION 2	//选项丢失；错误示例：list - -> List: please input option after '-'
#define REQUIRED_ARG 3		//参数缺失；错误示例：list -l -> List: please input argument for '-l'
#define WRONG_TYPE_ARG 4	//参数类型错误；错误示例：list -l a -> List: please input correct argument for '-l'

#define ERRMSG_MAXSIZE 100	//错误信息最大长度

//#define DEBUG

#if defined(__cplusplus)
extern "C" {
#endif

extern int optarg; //选项参数
extern int optind; //当前处理第几个选项
extern int opterr;	//选项错误码，无错误则为0


void get_errmsg(char *msg, char* const argv[]);
int my_getopt(int argc, char* const argv[], const char* optstring);
#if defined(__cplusplus)
}
#endif

#endif // MY_GETOPT_H