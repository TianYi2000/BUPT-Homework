#ifndef CNKS_CACHE_H_
#define CNKS_CACHE_H_
#include <stddef.h>
#include <assert.h>
#include <Windows.h>
#define MAX_MAP_HANDLE 16
#define BUFSIZE_OF_DOMAIN 256
#define BUFSIZE_OF_IP 16
typedef union {
    struct {
        unsigned char AA;
        unsigned char BB;
        unsigned char CC;
        unsigned char DD;
    };
    unsigned long to_u32;
} IPV4;
typedef struct {
    size_t index;
} MapHandle;
typedef struct PAIR_LIST Pair_List;
struct PAIR_LIST {
    const char* domain;
    const IPV4* ip;
    Pair_List* next;
};
enum {
    READ_MAP_EXIT_SUCCESS = 0,
    READ_MAP_EXIT_INVAILD_FORMAT,
    READ_MAP_EXIT_UNAVAILABLE_FILENAME,
    READ_MAP_EXIT_UNAVAILABLE_MAP_OBJECT
};
// 将IPV4转成点分十进制的字符串，字符串长度不会超出bufSize
extern void ipv4ToString_s(const IPV4* dot_dec, char* const buf, size_t bufSize);

extern const char* ipv4ToString(const IPV4* ipv4);
// 将字符串转为 IPV4
extern int stringToIPv4(const char* const buf, IPV4* dot_dec);
// 返回一个未使用过的MapHandle
extern void createMap(MapHandle* maphandle);
// 删除MapHandle
extern void deleteMap(MapHandle maphandle);
// 从文本文件中加载Map
extern int loadMapFromFile(const char* const filename, MapHandle maphandle);
// 在Map中寻找
extern int findDomainMap(const char* const domain, MapHandle maphandle, IPV4* result);
// 插入键值对
extern void insertMap(const char* const domain, const char* const ip, MapHandle maphandle);
// 获取由所有Pair连接成的单链表，NULL为end
extern Pair_List* getListMap(MapHandle maphandle);
// 释放链表内存
extern void FreeList(Pair_List* p);

typedef struct {
    char domain[BUFSIZE_OF_DOMAIN];
    char* packetBuffer;
    size_t packetBufferLength;
    size_t recentUseCount;
    IPV4 ipv4;
}PacketPair;

extern void insertPacket(PacketPair* const pairArray, size_t arraySize,const PacketPair*const insertElement, HANDLE Mutex);

extern PacketPair* findPacket(PacketPair* const pairArray, size_t arraySize, const char* const domainName);

extern void freePacket(PacketPair *const pair);
#endif //CNKS_CACHE_H_
