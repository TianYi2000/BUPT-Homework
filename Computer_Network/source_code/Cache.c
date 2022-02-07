#include "Cache.h"
#include "m_HashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


//返回初始化过的内存
#define M_MALLOC_N(type,num) (type*)calloc(num,sizeof(type))
#define M_MALLOC(type) (type*)calloc(1,sizeof(type))



static Map* mapHandles[MAX_MAP_HANDLE] = { 0 };
extern FILE* fpwirte;
extern int debug_level;
int AssignPairKey(void* _Dst, const void* const _Src) {
	return memcpy_s(_Dst, BUFSIZE_OF_DOMAIN, _Src, BUFSIZE_OF_DOMAIN);
}

int AssignPairValue(void* _Dst, const void* const _Src) {
	return memcpy_s(_Dst, sizeof(IPV4), _Src, sizeof(IPV4));
}

void* InitKey() {
	return M_MALLOC_N(char, 256);
}

void* InitValue() {
	return M_MALLOC(IPV4);
}

void FreeKey(void* p) {
	free(p);
}

void FreeValue(void* p) {
	free(p);
}
Map* GetpMapByHandle(MapHandle maphandle) {
	if (maphandle.index == 0 || maphandle.index >= MAX_MAP_HANDLE) return NULL;
	Map* pMap = mapHandles[maphandle.index];
	return pMap;
}

void ipv4ToString_s(const IPV4* const dot_dec, char* const buf, const size_t bufSize)
{
	size_t srcsize = BUFSIZE_OF_IP;
	char* srcbuf = M_MALLOC_N(char, srcsize);
	srcsize = sprintf_s(srcbuf, srcsize, "%hhu.%hhu.%hhu.%hhu", dot_dec->AA, dot_dec->BB, dot_dec->CC, dot_dec->DD);
	srcsize = min(srcsize + 1, bufSize);
	memcpy_s(buf, srcsize, srcbuf, srcsize);
	buf[srcsize - 1] = '\0';
	free(srcbuf);
}

const char* ipv4ToString(const IPV4* ipv4)
{
	char* buf = M_MALLOC_N(char, BUFSIZE_OF_IP);
	sprintf_s(buf, BUFSIZE_OF_IP, "%hhu.%hhu.%hhu.%hhu", ipv4->AA, ipv4->BB, ipv4->CC, ipv4->DD);
	return buf;
}

int stringToIPv4(const char* const buf, IPV4* dot_dec)
{
	return sscanf_s(buf, "%3hhu.%3hhu.%3hhu.%3hhu", &dot_dec->AA, &dot_dec->BB, &dot_dec->CC, &dot_dec->DD) != 4;
}

void createMap(MapHandle* pMapHandle)
{
	for (size_t i = 1; i < MAX_MAP_HANDLE; ++i) {
		if (!mapHandles[i]) {
			mapHandles[i] = M_MALLOC(Map);
			MapCreateInfo createinfo = {
				.max_load_factor = 0.75f,
				.pfnAssignKey = AssignPairKey,
				.pfnAssignValue = AssignPairValue,
				.pfnCmpKey = strcmp,
				.pfnInitKey = InitKey,
				.pfnFreeKey = FreeKey,
				.pfnInitValue = InitValue,
				.pfnFreeValue = FreeValue,
				.pfnHash = hash_Str
			};
			mapInit(mapHandles[i], &createinfo);
			pMapHandle->index = i;
			return;
		}
	}
	pMapHandle->index = 0;
}


int loadMapFromFile(const char* const filename, MapHandle maphandle)
{

	FILE* f = NULL;
	char domain[BUFSIZE_OF_DOMAIN] = { 0 }, ip[BUFSIZE_OF_IP] = { 0 };
	//open file
	errno_t e = fopen_s(&f, filename, "r");
	if (e) return READ_MAP_EXIT_UNAVAILABLE_FILENAME;
	//get pointer to map by handle
	Map* pMap = GetpMapByHandle(maphandle);
	if (pMap == NULL) return READ_MAP_EXIT_UNAVAILABLE_MAP_OBJECT;

	if (pMap->buckets == NULL) {
		int readNum = -1;
		int index = 0;
		while ((readNum = fscanf_s(f, "%s%s", domain, BUFSIZE_OF_DOMAIN, ip, BUFSIZE_OF_IP)) == 2) {
			if (debug_level >= 2) fprintf(fpwirte, "  %d:%s   %s\n", ++index, domain, ip);
			insertMap(domain, ip, maphandle);
		}
		if (readNum == 1) return READ_MAP_EXIT_INVAILD_FORMAT;
	}
	fclose(f);
	return READ_MAP_EXIT_SUCCESS;

}

void deleteMap(MapHandle maphandle)
{
	Map* pMap = GetpMapByHandle(maphandle);
	if (pMap) {
		mapFreeMap(pMap);
		free(pMap);
		mapHandles[maphandle.index] = NULL;
	}
}

int findDomainMap(const char* const domain, MapHandle maphandle, IPV4* result)
{
	Map* pMap = GetpMapByHandle(maphandle);
	if (pMap == NULL) return 0;
	else {
		Pair* ret = mapFind(pMap, domain);
		if (!ret) return 0;
		else {
			*result = *(IPV4*)ret->value;
			return 1;
		}
	}

}

void insertMap(const char* const domain, const char* const ip, MapHandle maphandle)
{
	Map* pMap = GetpMapByHandle(maphandle);
	if (!pMap) return;
	IPV4 IP;
	stringToIPv4(ip, &IP);
	Pair pair = mapMakePair(pMap, domain, &IP);
	mapInsert(pMap, &pair);
	mapFreePair(pMap, pair);
}

Pair_List* getListMap(MapHandle maphandle) {
	Map* pMap = GetpMapByHandle(maphandle);
	if (!pMap) return NULL;
	size_t idx;
	Pair* pair;
	Pair_List* ret = NULL;
	if (pair = mapBeginPair(pMap, &idx)) {
		Pair_List* beg = M_MALLOC(Pair_List);
		ret = beg;
		beg->domain = pair->key;
		beg->ip = pair->value;
		while (pair = mapNextPair(pMap, &idx, pair)) {
			beg->next = M_MALLOC(Pair_List);
			beg = beg->next;
			beg->domain = pair->key;
			beg->ip = pair->value;
		}
	}
	return ret;
}

void FreeList(Pair_List* p)
{
	if (p) {
		FreeList(p->next);
		free(p);
	}
}

void insertPacket(PacketPair* const pairArray, size_t arraySize, const PacketPair* const insertElement, HANDLE Mutex)
{
	WaitForSingleObject(Mutex, INFINITE);

	size_t findidx = 0;
	int maxUseCount = -1;
	for (size_t i = 0; i < arraySize; ++i) {
		if (((int)pairArray[i].recentUseCount) > maxUseCount) {
			findidx = i;
			maxUseCount = pairArray[i].recentUseCount;
		}
	}
	if (findPacket(pairArray,arraySize,insertElement->domain) == NULL) {
		freePacket(&pairArray[findidx]);
		pairArray[findidx] = *insertElement;
	}
	else {
		freePacket(insertElement);
	}

	ReleaseMutex(Mutex);
}

PacketPair* findPacket(PacketPair* const pairArray, size_t arraySize, const char* const domainName)
{
	assert(pairArray != NULL);
	PacketPair* ret = NULL;
	for (size_t i = 0; i < arraySize; ++i) {
		if (strcmp(domainName, pairArray[i].domain) == 0) {
			pairArray[i].recentUseCount = 0;
			ret = &pairArray[i];
		}
		else {
			++pairArray[i].recentUseCount;
		}
	}
	return ret;
}

void freePacket(PacketPair* const pair)
{
	if (pair->packetBuffer != NULL) {
		free(pair->packetBuffer);
	}	
}
