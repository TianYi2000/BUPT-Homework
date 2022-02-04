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

static char* optcursor = NULL;  //ѡ��ָ��('-'���һ���ַ���ָ��)

//�������ַ���ת��ΪԤ�ڵ���������(�ڱ��α����ҵ��ֻ���������͵Ĳ���)
int trans_int(char* const arg) {
    for (char* c = arg; *c != '\0'; c++) {
#ifdef DEBUG
        printf("DEBUG: ��ǰ��ʶ���Ƿ�Ϊ�����Ĳ����ַ�Ϊ:%c\n", *c);
#endif // DEBUG

        if (*c < '0' || *c > '9')
            return -1;
    }
    return atoi(arg);
}


int my_getopt(int argc, char* const argv[], const char* optstring) {
    int optchar = EOF;  //ѡ���ַ�����ȷ����ʱΪ����������ʱΪ'?'
    const char* optfind = NULL; //����ѡ��ģʽ�õ�������ָ��

        //�жϵ�ǰ�Ƿ���������ѡ��
    if (optind >= argc) {
#ifdef DEBUG
        debug("����������ѡ��,ѡ������ļ�����Ŀ¼��");
#endif // DEBUG
        return optchar;
    }

#ifdef DEBUG
    debug("OPTIND");
#endif // DEBUG

    //��ʼ�����Ĭ��ѡ���޲���
    optarg = -1;
    
    //ѡ��ǰ��'-'
    if (argv[optind][0] != '-') {
#ifdef DEBUG
        debug("ѡ��ǰ��-,����һ���ļ�����Ŀ¼��,�����������");
#endif // DEBUG

        return optchar;
    }
        
    //ѡ�ʧ��ѡ��ֻ����һ����
    if (strcmp(argv[optind], "-") == 0) {
#ifdef DEBUG
        debug("ѡ�ʧ��ѡ��ֻ����һ����");
#endif // DEBUG

        opterr = REQUIRED_OPTION;
        return optchar;
    }

    //��ʽ����ֹ����ѡ�����
    if (strcmp(argv[optind], "--") == 0) {
#ifdef DEBUG
        debug("��ȡ��--,��ʽ����ֹ����ѡ�����");
#endif // DEBUG

        optind++;
        return optchar;
    }

    //��ʼ����ѡ��
    optcursor = argv[optind] + 1;   //ѡ���1λΪ'-'����2λ��Ϊѡ���ַ�����˼�1

    //ѡ��������������ַ������ڱ��α����ҵ�����Ƕ�ѡ���һ�������в����ڵ�������ʲ��Ϸ�
    if (optcursor[1] != '\0') {
#ifdef DEBUG
        debug("ѡ��������������ַ�");
#endif // DEBUG
        opterr = INVALID_OPTION;
        return optchar = '?';
    }

    optchar = optcursor[0]; 
#ifdef DEBUG
    printf("DEBUG: ��ǰʶ���ѡ��Ϊ %c\n", optchar);
#endif // DEBUG

    optfind = strchr(optstring, optchar);
    if (optfind) {
        if (optfind[1] == ':') { //��ǰ�����õ���ģʽ��':'
             //��ǰѡ����Ҫ�Ĳ�������һ��argv��,optind+1
            if (++optind < argc) { //���������һ��argv
#ifdef DEBUG
                printf("DEBUG: ��ǰʶ�����Ϊ %s\n", argv[optind]);
#endif // DEBUG
                if ((optarg = trans_int(argv[optind])) == -1) {
#ifdef DEBUG
                    printf("DEBUG: ��ǰ����'%s'����һ������\n", argv[optind]);
#endif // DEBUG
                    opterr = WRONG_TYPE_ARG;
                    return optchar = '?';
                }
#ifdef DEBUG
                printf("DEBUG: ��ǰ����'%s'��һ������\n", argv[optind]);
#endif // DEBUG                   
            }
            else { //����Ѿ�����ȫ��argv,˵�����һ����Ҫѡ��Ĳ���û���������
#ifdef DEBUG
                debug("���һ����Ҫѡ��Ĳ���û���������");
#endif // DEBUG
                opterr = REQUIRED_ARG;
                return optchar = '?';
            }
            optcursor = NULL;   //��ǰѡ������
        }
    }
    else {  //���û���ڸ���ѡ���б����ҵ���ѡ�˵����ѡ��Ϸ�
#ifdef DEBUG
        debug("��ǰѡ��Ϸ�");
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
    case INVALID_OPTION:    //��Чѡ��
        strcat(msg, "invalid option '");
        strcat(msg, argv[optind]);
        strcat(msg, "'");
        break;
    case REQUIRED_OPTION:   //ѡ�ʧ
        strcat(msg, "please input option after '-'");
        break;
    case REQUIRED_ARG:      //����ȱʧ
        strcat(msg, "please input argument for '");
        strcat(msg, argv[optind - 1]);
        strcat(msg, "'");
        break;
    case WRONG_TYPE_ARG:    //�������ʹ���
        strcat(msg, "please input correct argument for '");
        strcat(msg, argv[optind - 1]);
        strcat(msg, "'");
        break;
    default:
        strcat(msg, "unkhnown error");
    }
}