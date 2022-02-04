#ifndef MY_GETOPT_H
#define MY_GETOPT_H

#define INVALID_OPTION 1	//��Чѡ�����ʾ����list -z -> List: invalid option '-z'
#define REQUIRED_OPTION 2	//ѡ�ʧ������ʾ����list - -> List: please input option after '-'
#define REQUIRED_ARG 3		//����ȱʧ������ʾ����list -l -> List: please input argument for '-l'
#define WRONG_TYPE_ARG 4	//�������ʹ��󣻴���ʾ����list -l a -> List: please input correct argument for '-l'

#define ERRMSG_MAXSIZE 100	//������Ϣ��󳤶�

//#define DEBUG

#if defined(__cplusplus)
extern "C" {
#endif

extern int optarg; //ѡ�����
extern int optind; //��ǰ����ڼ���ѡ��
extern int opterr;	//ѡ������룬�޴�����Ϊ0


void get_errmsg(char *msg, char* const argv[]);
int my_getopt(int argc, char* const argv[], const char* optstring);
#if defined(__cplusplus)
}
#endif

#endif // MY_GETOPT_H