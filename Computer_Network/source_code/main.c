#include "Cache.h"
#include "DNS.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <winsock.h>
#include <Windows.h>


#pragma comment(lib,"Ws2_32.lib")
#define LRU_CACHE_ARRAY_SIZE 8U
const unsigned int kTO = 5000U;
const char server[] = "127.0.0.1";
int debug_level = 0;
char DNS_SERVER[256] = "8.8.8.8";
char filename[256] = "dnsrelay.txt";
int query_count = 0;
FILE* fpwirte;

PacketPair LRUArray[LRU_CACHE_ARRAY_SIZE];

void InitWSA() {
    WSADATA wsaData;
    int startup_result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    assert(!startup_result);
}
typedef struct {
    char* recvbuf;
    MapHandle map;
    SOCKET socket;
    SOCKADDR_IN addr;
    HANDLE mutex;
}QueryCreateInfo;

/* Output the whole packet */
void Output_Packet(char* buf, int length) {
    unsigned char unit;
    fprintf(fpwirte, "Packet length = %d\n", length);
    fprintf(fpwirte, "Details of the package:\n");
    for (int i = 0; i < length; i++) {
        unit = (unsigned char)buf[i];
        fprintf(fpwirte, "%02x ", unit);
    }
    fprintf(fpwirte, "\n");
}

void queryFunc(char* const recvbuf, MapHandle map, SOCKET My_Socket,
    SOCKADDR_IN client_addr, HANDLE mutex) {
    HEADER header = *(HEADER*)recvbuf;
    const char* labeledName =
        recvbuf + sizeof(HEADER);  //报文头部紧接着的是label化的域名
    const char* domainNameBuffer = toNormalName(
        labeledName);  //将label域名转换为正常域名，用于本地名单查询
    if (debug_level) {
        /* Output time now */
        time_t t = time(NULL);
        char temp[128];
        strftime(temp, sizeof(temp), "%Y/%m/%d %X %A", localtime(&t));
        WaitForSingleObject(mutex, INFINITE);
        char client_ipv4[BUFSIZE_OF_IP]; 
        ipv4ToString_s(&client_addr.sin_addr.s_addr, client_ipv4, BUFSIZE_OF_IP);
        fprintf(fpwirte, "\n\n%4d %s  : ", ++query_count, temp);
        fprintf(fpwirte, "from %s:%5d ", client_ipv4, client_addr.sin_port);
        fprintf(fpwirte, "     %s\n", domainNameBuffer);
        ReleaseMutex(mutex);
    }

    UINT16 type =
        ntohs(*(UINT16*)(labeledName + strlen(domainNameBuffer) + 2));

    IPV4 result;
    size_t count = 0;
    char* p = recvbuf;

    /*若查询请求不是IPV4，返回域名服务器不支持查询类型*/
    if (type != 0x0001) {  // not Type A
        header.ans_num = 0;
        header.auth_num = 0;
        header.add_num = 0;
        header.flags = htons(packFLAGS(
            1, 0, 0, 0, 1, 1, 0, 4));  // RCODE = 4,域名服务器不支持查询类型
        memcpy(p, &header, sizeof(header));
        p += sizeof(header);
        p += strlen(domainNameBuffer) + 6; // labeledName + 2, 末尾4字节属性
    }
    /*如果在本地名单中查询到了该域名*/
    else if (findDomainMap(domainNameBuffer, map, &result)) {
        

        /*如果在查询到域名对应的地址为0.0.0.0,则向客户端返回"域名不存在"的报错消息*/
        if (result.to_u32 == 0) {
            header.ans_num = 0;
            header.auth_num = 0;
            header.add_num = 0;
            header.flags = htons(
                packFLAGS(1, 0, 0, 0, 1, 1, 0, 5));  // RCODE = 5,服务器拒绝应答
            memcpy(p, &header, sizeof(header));
            p += sizeof(header);
            p += strlen(domainNameBuffer) + 6; // labeledName + 2, 末尾4字节属性
            if (debug_level >= 2)
            {
                WaitForSingleObject(mutex, INFINITE);
                fprintf(fpwirte, "*****************WARNING! USER IS TRYING TO ACCESS A FORBIDDEN DOMAIN NAME!*****************\n");
                ReleaseMutex(mutex);
            }
        }
        /*应答普通响应报文*/
        else {

            if (debug_level >= 2)
            {
                WaitForSingleObject(mutex, INFINITE);
                char buf[20];
                ipv4ToString_s(&result, buf, sizeof(buf));
                fprintf(fpwirte, "     FIND DOMAINNAME in LOCAL FILE WHOSE IP IS %s\n", buf);
                ReleaseMutex(mutex);
            }

            count = 1;
            ANSWER_INFO answer;
            answer.aname = 0x0cc0;  // 2字节的省略形式，代表域名指针
                                    // （在网络端是c00c，本地变为0cc0），下同
            answer.aclass = 0x0100;
            answer.atype = 0x0100;
            answer.TTL = 0x40000000;
            answer.DL = 0x0400;
            header.add_num = 0;
            header.flags = htons(packFLAGS(1, 0, 0, 0, 1, 1, 0, 0));
            header.ans_num = htons(count);
            memcpy(p, &header, sizeof(header));
            p += sizeof(header);
            p += 4 + strlen(labeledName) + 1;
            memcpy(p, &answer, sizeof(ANSWER_INFO));
            p += sizeof(ANSWER_INFO);
            memcpy(p, &result, sizeof(IPV4));
            p += sizeof(IPV4);

        }

    }
    /*表中未检到该域名，则向因特网DNS服务器发出查询，并将结果返给客户端*/
    else {
        WaitForSingleObject(mutex, INFINITE);
        PacketPair* nowPair = findPacket(LRUArray, LRU_CACHE_ARRAY_SIZE, domainNameBuffer);
        if (nowPair == NULL) {
            ReleaseMutex(mutex);

            /*向因特网DNS服务器发送查询报文*/
            if (debug_level >= 2)   
                fprintf(fpwirte, "     NO DOMAINNAME in LACAL, TRYING TO SEND REQUEST TO OUTER SERVER\n");

            IPV4* results = NULL;
            size_t anslen = 0;
            results = resolveDomainName(domainNameBuffer, strlen(domainNameBuffer),
                p, &anslen, &count);


            ((HEADER*)p)->id = header.id;
            p += anslen;
            if (count > 0) {
                PacketPair newPair = { 0 };
                strcpy(newPair.domain, domainNameBuffer);
                newPair.packetBufferLength = p - recvbuf;
                newPair.packetBuffer = calloc(newPair.packetBufferLength, sizeof(char));
                memcpy(newPair.packetBuffer, recvbuf, newPair.packetBufferLength);
                newPair.ipv4 = results[0];
                insertPacket(LRUArray, LRU_CACHE_ARRAY_SIZE, &newPair, mutex);

                if (debug_level >= 2)
                {
                    char buf[20];
                    ipv4ToString_s(&newPair.ipv4, buf, sizeof(buf));
                    fprintf(fpwirte, "     RECEIVE IP %s FOR DOMAIN NAME %s FROM OUTER SERVER\n", buf, domainNameBuffer);
                    fprintf(fpwirte, "     STORE PAIRLIST (%s , %s) TO CACHE\n", domainNameBuffer, buf);
                }

            }
            free(results);
        }
        else {
            if (debug_level >= 2)
            {
                char buf[20];
                ipv4ToString_s(&nowPair->ipv4, buf, sizeof(buf));
                fprintf(fpwirte, "     FIND DOMAINNAME in CACHE WHOSE IP IS %s\n", buf);
            }
            memcpy(recvbuf, nowPair->packetBuffer, nowPair->packetBufferLength);
            p += nowPair->packetBufferLength;
            ReleaseMutex(mutex);
        }
    }
    free(domainNameBuffer);
    WaitForSingleObject(mutex, INFINITE);
    sendto(My_Socket, recvbuf, p - recvbuf, 0, &client_addr,
        sizeof(client_addr));
    ReleaseMutex(mutex);
    if (debug_level >= 2)
    {
        char buf[20];
        ipv4ToString_s(&client_addr.sin_addr.s_addr, buf, sizeof(buf));
        fprintf(fpwirte, "     SEND TO %s:%5d,[%d bytes]\n", buf, client_addr.sin_port ,p - recvbuf);
        WaitForSingleObject(mutex, INFINITE);
        fprintf(stdout, "     ------------------LRU------------------\n");

        for (size_t i = 0; i < LRU_CACHE_ARRAY_SIZE; ++i) {
            if (LRUArray[i].packetBufferLength > 0) {
                ipv4ToString_s(&LRUArray[i].ipv4, buf, sizeof(buf));
                fprintf(stdout, "     %s -> %s\n", LRUArray[i].domain, buf);
            }

        }
        fprintf(stdout, "     ---------------------------------------\n");
        ReleaseMutex(mutex);
    }
}
DWORD WINAPI pt_Query(void* arg) {
    QueryCreateInfo* Qarg = arg;
    char buf[10240];
    memcpy(buf, Qarg->recvbuf, 1024);
    queryFunc(buf, Qarg->map, Qarg->socket, Qarg->addr, Qarg->mutex);
    free(arg);
    return 0;
}
void Process_Parameters(int argc, char* argv[]) {
    time_t t = time(NULL);
    char temp[128];
    strftime(temp, sizeof(temp), "%Y/%m/%d %X %A", localtime(&t));
    fprintf(fpwirte, "DNS SERVER v1.0 BUILD on %s  \n", temp);
    int user_set_dns_flag = 0;
    if (argc > 1 && argv[1][0] == '-') {
        if (argv[1][1] == 'd') debug_level++; /* Debug level add to 1 */
        if (argv[1][2] == 'd') debug_level++; /* Debug level add to 2 */
        if (argc > 2) {
            user_set_dns_flag = 1; /* If user set the dns server ip address */
            memcpy(DNS_SERVER, argv[2], strlen(argv[2]));
        }
        if (argc > 3) {
            memcpy(filename, argv[3], strlen(argv[3]));
        }
    }
    if (user_set_dns_flag) /* If user set the dns server ip address */
        fprintf(fpwirte, "Set DNS server : %s\n", argv[2]);
    else /* If user do not set the dns server ip address, set it by default */
        fprintf(fpwirte, "Set DNS server : %s by default\n", DNS_SERVER);
    if (argc > 3)
        fprintf(fpwirte, "read file from: %s\n", filename);
    else
        fprintf(fpwirte, "read file from: %s by default\n", filename);
    fprintf(fpwirte, "Debug level : %d\n", debug_level);
}
int main(int argc, char* argv[]) {
    errno_t err;
    err = fopen_s(&fpwirte, "log2.txt", "w");
    if (err != 0) {
        fprintf(stderr, "Can not Create File log2.txt !");
        return EXIT_FAILURE;
    }
    fpwirte = stdout;
    Process_Parameters(argc, argv);
    MapHandle map;
    createMap(&map);
    if (loadMapFromFile(filename, map)) {
        fprintf(stderr, "Can not load Map from %s!",filename);
        return EXIT_FAILURE;
    }
    SOCKADDR_IN bind_port_addr = { 0 };
    bind_port_addr.sin_family = AF_INET;                          //IPv4
    bind_port_addr.sin_port = htons(53);                        //DNS53号端口
    InitWSA();
    SOCKADDR_IN client_addr = { 0 };
    int fromlen = sizeof(client_addr);
    SOCKET My_Socket = INVALID_SOCKET;
    My_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //建立分配UDP套结字
    assert(My_Socket != INVALID_SOCKET);
    setsockopt(My_Socket, SOL_SOCKET, SO_RCVTIMEO, &kTO, sizeof(kTO)); //设置接收超时时间
    if (debug_level) fprintf(fpwirte, "set runout time :%d ms\n", kTO);
    if (bind(My_Socket, &bind_port_addr, sizeof(bind_port_addr)))
    {
        printf("绑定失败！\n");
        return EXIT_FAILURE;
    }
    char recvbuf[10240];
    HANDLE mainMutex = CreateMutex(NULL, FALSE, NULL);
    for (;;) {
        int len = recvfrom(My_Socket, recvbuf, 10240, 0, &client_addr, &fromlen);
        if (len > 0) {
            QueryCreateInfo* ci = calloc(1, sizeof(*ci));
            if (ci == NULL) {
                return EXIT_FAILURE;
            }
            *ci = (QueryCreateInfo){ .socket = My_Socket,.addr = client_addr,.map = map,.recvbuf = recvbuf,.mutex = mainMutex };
            if (!CreateThread(NULL, 0, pt_Query, ci, 0, NULL)) {
                fprintf(stderr, "Can not Create Thread !");
                return EXIT_FAILURE;
            }
        }
    }

    deleteMap(map);
}