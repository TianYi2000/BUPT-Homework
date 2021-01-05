#include "Algorithm4.h"

#include <cstdlib>
#include <ctime>

Algorithm4::Algorithm4(int n, int* data)
{
	N = n;
	Data = data;
}

long long Algorithm4::Run()
{
	long long StartTime, EndTime;
	StartTime = GetSysTimeMicros();
	long long ThisSum, MaxSum;
	int i, j;
	ThisSum = MaxSum = 0;
	for (j = 0; j < N; j++) {
		ThisSum += Data[j];
		if (ThisSum > MaxSum)
			MaxSum = ThisSum;
		else if (ThisSum < 0)
			ThisSum = 0;
	}
	EndTime = GetSysTimeMicros();
	RunTime = EndTime - StartTime;
	return RunTime;
}

Algorithm4::~Algorithm4()
{

}

