#include "Algorithm1.h"
#include <cstdlib>
#include <ctime>

Algorithm1::Algorithm1(int n, int* data)
{
	N = n;
	Data = data;
}

long long Algorithm1::Run()
{
	long long StartTime, EndTime;
	StartTime = GetSysTimeMicros();
	long long ThisSum, MaxSum;
	int i, j, k;
	MaxSum = 0;
	for (i = 0; i < N; i++) 
	for (j = i; j < N; j++) {
		ThisSum = 0;
		for (k = i; k <= j; k++)
			ThisSum += Data[k];
			if (ThisSum > MaxSum)
				MaxSum = ThisSum;
		}
	EndTime = GetSysTimeMicros();
	RunTime = EndTime - StartTime;
	return RunTime;
}

Algorithm1::~Algorithm1()
{

}

