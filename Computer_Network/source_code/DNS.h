#ifndef _CNKS_DNS_H_
#define _CNKS_DNS_H_
#include <winsock.h>
#include <assert.h>
#define MAX_PACKAGE_SIZE (1024 * 1024)
#pragma pack(1)
/*
DNS��ͷ
*/
typedef struct
{
    UINT16 id;          // �Ự��ʶ
    UINT16 flags;       // ��־
    UINT16 q_num;     // ������ 
    UINT16 ans_num;   // ��Դ��¼��
    UINT16 auth_num;  // ��Ȩ��Դ��¼��
    UINT16 add_num;   // ������Դ��¼��
}HEADER;


/*
��ѯ��������
*/
typedef struct {
    UINT16 qtype;       //��ѯ����
    UINT16 qclass;      //��ѯ��

} QUESTION_INFO;

/*
�ش�����
*/
typedef struct {
    UINT16 aname;        //����
    UINT16 atype;        //��ѯ����
    UINT16 aclass;       //��ѯ��
    UINT32 TTL;         //����ʱ��
    UINT16 DL;          //��Դ���ݳ���
} ANSWER_INFO;
#pragma pack()
extern unsigned char* TransName(const unsigned char* name, int len);
extern IPV4* resolveDomainName(const unsigned char* name, int len, char* answers, size_t* answerslen, size_t* pResultsCount);
extern const char* toNormalName(const char* labeledName);
#endif

