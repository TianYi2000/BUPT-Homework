#ifndef _CNKS_DNS_H_
#define _CNKS_DNS_H_
#include <winsock.h>
#include <assert.h>
#define MAX_PACKAGE_SIZE (1024 * 1024)
#pragma pack(1)
/*
DNS报头
*/
typedef struct
{
    UINT16 id;          // 会话标识
    UINT16 flags;       // 标志
    UINT16 q_num;     // 问题数 
    UINT16 ans_num;   // 资源记录数
    UINT16 auth_num;  // 授权资源记录数
    UINT16 add_num;   // 额外资源记录数
}HEADER;


/*
查询问题区域
*/
typedef struct {
    UINT16 qtype;       //查询类型
    UINT16 qclass;      //查询类

} QUESTION_INFO;

/*
回答区域
*/
typedef struct {
    UINT16 aname;        //域名
    UINT16 atype;        //查询类型
    UINT16 aclass;       //查询类
    UINT32 TTL;         //生存时间
    UINT16 DL;          //资源数据长度
} ANSWER_INFO;
#pragma pack()
extern unsigned char* TransName(const unsigned char* name, int len);
extern IPV4* resolveDomainName(const unsigned char* name, int len, char* answers, size_t* answerslen, size_t* pResultsCount);
extern const char* toNormalName(const char* labeledName);
#endif

