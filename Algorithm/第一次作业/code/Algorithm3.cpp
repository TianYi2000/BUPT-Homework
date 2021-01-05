#include "Algorithm3.h"
#include <cstdlib>
#define gmax(_a, _b) ((_a) > (_b) ? (_a) : (_b))

Algorithm3::Algorithm3(int n, int* data)
{
	N = n;
	Data = data;
}

long long Algorithm3::Run()
{
	long long StartTime, EndTime;
    StartTime = GetSysTimeMicros();
    Work(1, N);
	EndTime = GetSysTimeMicros();
	RunTime = EndTime - StartTime;
	return RunTime;
}

long long Algorithm3::Work(int left, int right)
{
    if (right == left)
        return gmax(Data[left], 0);

    int center = (left + right) / 2; 
    long long LeftMax = Work(left, center);
    long long RightMax = Work(center + 1, right);
    long long sum = 0;
    long long left_max = 0;
    for (int i = center; i >= left; i--)
    {
        sum += Data[i];
        left_max = gmax(left_max, sum);
    }
    sum = 0;
    long long right_max = 0;
    for (int i = center + 1; i <= right; i++)
    {
        sum += Data[i];
        right_max = gmax(right_max, sum);
    }
    return gmax(gmax(LeftMax, RightMax), left_max + right_max);
}

Algorithm3::~Algorithm3()
{

}

