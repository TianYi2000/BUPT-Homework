#include "Algorithm2.h"
#include <cstdlib>
#include <ctime>

Algorithm2::Algorithm2(int n, int* data)
{
	N = n;
	Data = data;
}

long long Algorithm2::Run()
{
	long long StartTime, EndTime;
	StartTime = GetSysTimeMicros();
	long long ThisSum, MaxSum;
	int i, j, k;
	MaxSum = 0;
	for (i = 0; i < N; i++) {
		ThisSum = 0;
		for (j = i; j < N; j++) { 
			ThisSum += Data[j];
			if (ThisSum > MaxSum)
			MaxSum = ThisSum;
		}
	}
	EndTime = GetSysTimeMicros();
	RunTime = EndTime - StartTime;
	return RunTime;
}

Algorithm2::~Algorithm2()
{

}

