#pragma once
#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif  // _WIND32

// ¶¨Òå64Î»ÕûÐÎ
#if defined(_WIN32) && !defined(CYGWIN)
typedef __int64 int64_t;
#else
typedef long long int64t;
#endif  // _WIN32

int64_t GetSysTimeMicros();