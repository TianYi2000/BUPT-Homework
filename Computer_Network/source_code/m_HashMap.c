#include "m_HashMap.h"
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#if defined(_WIN64)
const size_t _FNV_offset_basis = 14695981039346656037ULL;
const size_t _FNV_prime = 1099511628211ULL;
#else // defined(_WIN64)
const size_t _FNV_offset_basis = 2166136261U;
const size_t _FNV_prime = 16777619U;
#endif // defined(_WIN64)

// From MSVC 16.0
inline size_t _Fnv1a_append_bytes(size_t _Val, const unsigned char* const _First, const size_t _Count) { // accumulate range [_First, _First + _Count) into partial FNV-1a hash _Val
    for (size_t _Idx = 0; _Idx < _Count; ++_Idx) {
        _Val ^= (size_t)(_First[_Idx]);
        _Val *= _FNV_prime;
    }

    return _Val;
}

// From MSVC 16.0
inline size_t _Hash_array_representation(const unsigned char* const _First, const size_t _Count) { // bitwise hashes the representation of an array
    return _Fnv1a_append_bytes(
        _FNV_offset_basis, (const unsigned char*)(_First), _Count);
}

inline size_t hash_Str(const char* const str) {

    return _Hash_array_representation(str, strlen(str));

}

// 分配并初始化一段Pair的内存，并给key和value分配内存并初始化
Pair* _map_Allocate_n_Pairs(Map* map, size_t num) {
    Pair* ret = calloc(num, sizeof(*ret));
    for (size_t i = 0; i < num; ++i) {
        ret[i].key = map->pfnInitKey();
        ret[i].value = map->pfnInitValue();
    }
    return ret;
}
// 释放key和value上的内存，然后释放Pair的内存
void _map_Free_n_Pairs(Map* map, Pair* pair, size_t num) {
    for (size_t i = 0; i < num; ++i) {
        map->pfnFreeKey(pair[i].key);
        map->pfnFreeValue(pair[i].value);
    }
    free(pair);
}

inline float mapLoadFactor(Map* map)
{
    return 1.0f * map->size / map->bucket_count;
}

inline void mapInit(Map* map, const MapCreateInfo* const createinfo)
{
    assert(!isnan(createinfo->max_load_factor) && createinfo->max_load_factor > 0);
    assert(createinfo->pfnAssignKey != NULL);
    assert(createinfo->pfnAssignValue != NULL);
    assert(createinfo->pfnCmpKey != NULL);
    assert(createinfo->pfnFreeKey != NULL);
    assert(createinfo->pfnFreeValue != NULL);
    assert(createinfo->pfnHash != NULL);
    assert(createinfo->pfnInitKey != NULL);
    assert(createinfo->pfnInitValue != NULL);
    map->max_load_factor = createinfo->max_load_factor;
    map->pfnAssignKey = createinfo->pfnAssignKey;
    map->pfnAssignValue = createinfo->pfnAssignValue;
    map->pfnCmpKey = createinfo->pfnCmpKey;
    map->pfnFreeKey = createinfo->pfnFreeKey;
    map->pfnFreeValue = createinfo->pfnFreeValue;
    map->pfnHash = createinfo->pfnHash;
    map->pfnInitKey = createinfo->pfnInitKey;
    map->pfnInitValue = createinfo->pfnInitValue;
}

Pair* mapBeginPair(Map* map, size_t* idx)
{
    for (size_t i = 0; i < map->bucket_count; ++i) {
        if (map->buckets[i].next) {
            *idx = i;
            return &map->buckets[i];
        }
    }
    return NULL;
}

Pair* mapNextPair(Map* map, size_t* idx, Pair* lastPair) {
    if (lastPair == NULL) return NULL;
    Pair* ret = NULL;
    if (lastPair->next) ret = lastPair->next;
    else ret = lastPair;
    while (ret && ret->next == NULL) {
        if (*idx == map->bucket_count - 1) ret = NULL;
        else ret = &map->buckets[++(*idx)];
    }
    return ret;

}

void mapResize(Map* map) {
    if (map->bucket_count == 0 || mapLoadFactor(map) > map->max_load_factor) {
        size_t new_bucket_count = max(2 * map->bucket_count, (size_t)ceil(1.0f / map->max_load_factor));
        Pair* new_pPairs = _map_Allocate_n_Pairs(map, new_bucket_count);
        //遍历原来的整个map
        size_t cidx = 0;
        for (Pair* cpair = mapBeginPair(map, &cidx); cpair != NULL; cpair = mapNextPair(map, &cidx, cpair)) {
            size_t currentNewIndex = map->pfnHash(cpair->key) % new_bucket_count;
            Pair* currentPair = &new_pPairs[currentNewIndex];
            while (currentPair->next) currentPair = currentPair->next;
            map->pfnAssignKey(currentPair->key, cpair->key);
            map->pfnAssignValue(currentPair->value, cpair->value);
            currentPair->next = _map_Allocate_n_Pairs(map, 1);
        }
        mapFreeMap(map);

        map->bucket_count = new_bucket_count;
        map->buckets = new_pPairs;
    }
}

void mapInsert(Map* map, const Pair* const pair) {
    mapResize(map);
    size_t hashedIdx = map->pfnHash(pair->key) % map->bucket_count;
    Pair* pInsertPair = &map->buckets[hashedIdx];
    while (pInsertPair->next) pInsertPair = pInsertPair->next;
    map->pfnAssignKey(pInsertPair->key, pair->key);
    map->pfnAssignValue(pInsertPair->value, pair->value);
    pInsertPair->next = _map_Allocate_n_Pairs(map, 1);
    ++map->size;
}

Pair* mapFind(Map* map, const char* const keyToFind) {
    size_t currentNewIndex = map->pfnHash(keyToFind) % map->bucket_count;
    for (Pair* currentPair = &map->buckets[currentNewIndex]; currentPair; currentPair = currentPair->next) {
        if (map->pfnCmpKey(keyToFind, currentPair->key) == 0) {
            return currentPair;
        }
    }
    return NULL;
}

Pair mapMakePair(Map* map, const void* const key, const void* const value)
{
    Pair ret = { .next = NULL, .key = map->pfnInitKey(),.value = map->pfnInitValue() };
    map->pfnAssignKey(ret.key, key);
    map->pfnAssignValue(ret.value, value);
    return ret;
}

void mapFreePair(Map* map, Pair pair) {
    map->pfnFreeKey(pair.key);
    map->pfnFreeValue(pair.value);
}

void mapFreeMap(Map* map)
{
    if (map->buckets) {
        // 先删除桶上的链表，因为链表是一个一个分配的
        for (size_t i = 0; i < map->bucket_count; ++i) {
            Pair* currentPair = map->buckets[i].next;
            while (currentPair != NULL) {
                Pair* pairWaitFree = currentPair;
                currentPair = currentPair->next;
                _map_Free_n_Pairs(map, pairWaitFree, 1);
            }
        }
        // 最后删除整个桶，因为桶是连续分配的数组
        _map_Free_n_Pairs(map, map->buckets, map->bucket_count);
        map->buckets = NULL;
    }
}
