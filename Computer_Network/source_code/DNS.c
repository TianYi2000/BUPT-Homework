#define  _CRT_SECURE_NO_WARNINGS
#pragma comment(lib,"Ws2_32.lib")
#include <stdio.h> 
#include <string.h>    
#include <stdlib.h>    
#include <winsock.h>
#include <assert.h>
#include "Cache.h"
#include "DNS.h"

//#define DEBUG


#define QUERY_FLAGS 0,0,0,0,1,0,0,0     //查询报文的FLAGS各字段默认值
#define TYPE_A 1
#define CLASS_IN 1

extern char DNS_SERVER[];  //= "8.8.8.8";  // DNS服务器地址
extern FILE* fpwirte;
extern int debug_level;

const unsigned int kTimeOut = 5000U;
/*
将组装好的报文部件拷贝到DNS完整报文上
*/
void copytoDNS(UINT8** const dst, const UINT8* src, int len)
{
    memcpy(*dst, src, len);
    *dst += len;
}


UINT16 packFLAGS(UINT8 QR, UINT8 OPCODE, UINT8 AA, UINT8 TC, UINT8 RD, UINT8 RA, UINT8 Z, UINT8 RCODE)
{
    UINT16 flags = 0;
    flags += QR;         //QR=0 操作类型为查询报文
    flags = (flags << 4) + OPCODE; //OPCODE=0 查询类型为标准查询
    flags = (flags << 1) + AA; //AA=0 不置位,该域名解析服务器不授权回答该域
    flags = (flags << 1) + TC; //TC=0 不置位,报文不可被截断
    flags = (flags << 1) + RD; //RD=1 表示希望域名解析服务器采取迭代解析
    flags = (flags << 1) + RA; //RA=0 表示域名解析服务器采取迭代解析
    flags = (flags << 3) + Z; //Z字段全部置0,保留未用
    flags = (flags << 4) + RCODE; //RCODE=0,响应类型为无差错
    return flags;
}


int sendtoDNSServer(const UINT8* buf, int len, SOCKET my_socket, SOCKADDR_IN* const dns_addr)
{
    /*发送报文*/
    if (sendto(my_socket, buf, len, 0, dns_addr, sizeof(*dns_addr)) < 0)
    {
        return 0;
    }
    return 1;
}

void recvfromDNSServer(UINT8* buf, unsigned int* len, SOCKET my_socket, SOCKADDR_IN* const dns_addr)
{

    int addr_len = sizeof(*dns_addr);
    *len = recvfrom(my_socket, buf, 1024, 0, dns_addr, &addr_len);
    if (debug_level >= 2) {
        char buf[20];
        ipv4ToString_s(&dns_addr->sin_addr.S_un, buf, sizeof(buf));
        fprintf(fpwirte, "\n     reveive packet from extern DNS:%s\n", buf);
    }
}

/*
域名转换为查询名
域名被编码为一些labels序列，每个labels包含一个字节表示后续字符串长度，以及这个字符串，以0长度和空字符串来表示域名结束
例:www.bupt.edu.cn -> 3www4bupt3edu2cn0
*/
unsigned char* TransName(const unsigned char* name, int len) {
    unsigned char* qname = malloc(len + 2);  //labels的数量比域名中的'.'多2
    int lastpos = 0;
    for (int i = 0; i < len; ++i)
    {
        if (name[i] == '.')
        {
            qname[lastpos] = i - lastpos;
            lastpos = i + 1;
        }
        else if (i == len - 1)
        {
            qname[lastpos] = i + 1 - lastpos;
            qname[i + 1] = name[i];
        }
        else
        {
            qname[i + 1] = name[i];
        }
    }

    qname[len + 1] = 0;       //以0长度和空字符串来表示域名结束
    /*
    #if !defined(NDEBUG)
    fprintf(fpwirte, "TransName Result is:%s\n", qname);
#endif
    */

    return qname;
}


/*
解析未知域名
*name是待查询的域名,len为域名字符串的长度
*/
/* Output the whole packet */
void Output_Packet2(char* buf, int length) {
    unsigned char unit;
    fprintf(fpwirte, "     Packet length = %d\n", length);
    fprintf(fpwirte, "     Details of the package:\n     ");
    for (int i = 0; i < length; i++) {
        unit = (unsigned char)buf[i];
        fprintf(fpwirte, "%02x ", unit);
    }
    fprintf(fpwirte, "\n");
}


IPV4* resolveDomainName(const unsigned char* name, int len,
    char* answers,
    size_t* answerslen, size_t* pResultsCount)
{
    UINT8* buf = calloc(MAX_PACKAGE_SIZE, sizeof(UINT8));    //存储完整DNS报文
    UINT8* pbuf = buf;  //指向当前即将拷贝报文部件的地址

    /*设置DNS报文首部*/
    HEADER header;

    header.id = (UINT16)htons(getpid());  // id设为进程标识符
    header.flags = htons(packFLAGS(QUERY_FLAGS));
    header.q_num = htons(1);  // 1个问题
    header.ans_num = 0;
    header.auth_num = 0;
    header.add_num = 0;

    copytoDNS(&pbuf, (UINT8*)&header, sizeof(header));

    /*设置DNS查询问题*/
    UINT8* qname = TransName(name, len);  //查询名称为转换过的域名
    copytoDNS(&pbuf, qname, len + 2);
    free(qname);

    QUESTION_INFO questionInfo;

    questionInfo.qtype = htons(TYPE_A);  //查询类型为A,代表查询IP地址
    questionInfo.qclass = htons(CLASS_IN);  //查询的协议类为IN,代表Internet

    copytoDNS(&pbuf, (UINT8*)&questionInfo, sizeof(header));
    SOCKET my_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //建立分配UDP套结字
    assert(my_socket != INVALID_SOCKET);
    SOCKADDR_IN dns_addr = { 0 };
    dns_addr.sin_family = AF_INET;                          //IPv4
    dns_addr.sin_port = htons(53);                          //DNS53号端口
    dns_addr.sin_addr.s_addr = inet_addr(DNS_SERVER);       //DNS服务器IP 
    setsockopt(my_socket, SOL_SOCKET, SO_RCVTIMEO, &kTimeOut,
        sizeof(kTimeOut));  //设置接收超时时间
    /*向DNS服务器发送DNS请求报文*/
    if (!sendtoDNSServer(buf, pbuf - buf, my_socket, &dns_addr)) {
        if (debug_level >= 2)
            fprintf(fpwirte, "send to DNS Server fail！\n");
        free(buf);
        return NULL;
    }
    /*从DNS服务器接收报文*/
    int trys = 0, recvbuflen = 0;
    while (trys <= 3) {
        recvbuflen = 0;
        recvfromDNSServer(buf, &recvbuflen, my_socket, &dns_addr);

        //超时之后重传
        if (recvbuflen < 0) {
            if (debug_level >= 2)
                fprintf(fpwirte, "reveive overtime from DNS server---%dth retransmission\n", trys);
            if (!sendtoDNSServer(buf, pbuf - buf, my_socket, &dns_addr)) {
                if (debug_level >= 2)
                    fprintf(fpwirte, "send to DNS Server fail！\n");
                free(buf);
                return;
            }
        }
        else {
            break;
        }
        ++trys;
    }
    /*3次重传均失败*/
    if (recvbuflen < 0) {
        if (debug_level >= 2)
            fprintf(fpwirte, "3 time retransmission all failed!\n");
        header.ans_num = 0;
        header.auth_num = 0;
        header.add_num = 0;
        header.flags = htons(
            packFLAGS(1, 0, 0, 0, 1, 1, 0, 2));  //设置RCODE = 2,服务器失败

        if (answers != NULL) {
            memcpy(answers, &header, sizeof(header));
        }
        if (answerslen != NULL) {
            *answerslen = sizeof(header);
        }
        free(buf);
        return NULL;
    }

    /*分析接收到的DNS报文*/
    pbuf = buf;  // pbuf指向报文头部
    HEADER* rheader = pbuf;
    if (answers != NULL) {
        *answerslen = recvbuflen;
        memcpy(answers, buf, *answerslen);
    }

    pbuf = pbuf + sizeof(HEADER) + len + 2 +
        sizeof(QUESTION_INFO);  // pbuf指向报文Answer部分

    size_t count = ntohs(rheader->ans_num);
    *pResultsCount = 0;
    IPV4* result = calloc(count, sizeof(IPV4));
    for (int i = 0; i < count; i++) {
        ANSWER_INFO* ranswer = pbuf;
        pbuf = pbuf + sizeof(ANSWER_INFO);

        if (ntohs(ranswer->atype) == TYPE_A) {
            if (result != NULL) {
                result[*pResultsCount] = *(IPV4*)pbuf;
            }
            pbuf += sizeof(IPV4);
            ++(*pResultsCount);
        }
        else
            pbuf = pbuf + ntohs(ranswer->DL);
    }
    if (debug_level >= 2)
        Output_Packet2(buf, pbuf - buf);
    free(buf);
    return result;
}

const char* toNormalName(const char* labeledName)
{
    char* Raw_buf = calloc(200, sizeof(char));
    size_t count;
    char* buf = Raw_buf;
    while (count = *(labeledName++)) {
        memcpy(buf, labeledName, count);
        labeledName += count;
        buf += count;
        *(buf++) = '.';
    }
    *(buf - 1) = 0;
    return Raw_buf;
}
