#ifndef CNKS_M_HASHMAP_H_
#define CNKS_M_HASHMAP_H_

#include <string.h>


typedef struct PAIR Pair;
struct PAIR {
    //用void* 来解决C中的泛型问题
    void* key;
    void* value;
    Pair* next;
};
typedef struct {
    //最大负载因子
    float max_load_factor;
    //函数指针，由外部定义如何比较key，返回值：负值为lhs < rhs, 0 为 lhs == rhs ，正值为lhs > rhs
    int (*pfnCmpKey)(const void* const lhs, const void* const rhs);
    //函数指针，由外部定义如何给key赋值，返回值：非0为有错误发生
    int (*pfnAssignKey)(void* _Dst, const void* const _Src);
    //函数指针，由外部定义如何给value赋值，返回值：非0为有错误发生
    int (*pfnAssignValue)(void* _Dst, const void* const _Src);
    //函数指针，由外部定义如何给key分配内存，返回值：分配的内存地址
    void* (*pfnInitKey)();
    //函数指针，由外部定义如何释放key的内存
    void (*pfnFreeKey)(void* p);
    //函数指针，由外部定义如何给value分配内存，返回值：分配的内存地址
    void* (*pfnInitValue)();
    //函数指针，由外部定义如何释放value的内存
    void (*pfnFreeValue)(void* p);
    //函数指针，由外部定义key的hash函数
    size_t(*pfnHash)(const void* const key);
    //指向桶数组的指针
    Pair* buckets;
    size_t size;
    size_t bucket_count;
} Map;

typedef struct {
    //最大负载因子
    float max_load_factor;
    //函数指针，由外部定义如何比较key，返回值：负值为lhs < rhs, 0 为 lhs == rhs ，正值为lhs > rhs
    int (*pfnCmpKey)(const void* const lhs, const void* const rhs);
    //函数指针，由外部定义如何给key赋值，返回值：非0为有错误发生
    int (*pfnAssignKey)(void* _Dst, const void* const _Src);
    //函数指针，由外部定义如何给value赋值，返回值：非0为有错误发生
    int (*pfnAssignValue)(void* _Dst, const void* const _Src);
    //函数指针，由外部定义如何给key分配内存，返回值：分配的内存地址
    void* (*pfnInitKey)();
    //函数指针，由外部定义如何释放key的内存
    void (*pfnFreeKey)(void* p);
    //函数指针，由外部定义如何给value分配内存，返回值：分配的内存地址
    void* (*pfnInitValue)();
    //函数指针，由外部定义如何释放value的内存
    void (*pfnFreeValue)(void* p);
    //函数指针，由外部定义key的hash函数
    size_t(*pfnHash)(const void* const key);
} MapCreateInfo;


//返回字符串的hash值
extern inline size_t hash_Str(const char* const str);

//Map 的负载因子
extern inline float mapLoadFactor(Map* map);

extern inline void mapInit(Map* map,const MapCreateInfo *const createinfo);

//返回 第一个非空桶以及它的下标，如无则返回NULL
extern Pair* mapBeginPair(Map* map, size_t* idx);

//遍历 idx为桶的下标 lastPair为当前Pair指针，终止态为NULL
extern Pair* mapNextPair(Map* map, size_t* idx, Pair* lastPair);

//判断HashMap的桶是否需要扩容
extern void mapResize(Map* map);

//在HashMap中插入键值对
extern void mapInsert(Map* map, const Pair* const pair);

//通过相应的键在HashMap中寻找，若找不到，返回NULL
extern Pair* mapFind(Map* map, const char* const keyToFind);

//返回一个由key和value构造的Pair，需要手动调用mapFreePair释放键值的内存
extern Pair mapMakePair(Map* map, const void* const key, const void* const value);

//释放键和值占用的内存
extern void mapFreePair(Map* map, Pair pair);

//释放map分配给桶的内存
extern void mapFreeMap(Map* map);
#endif //CNKS_M_HASHMAP_H_
