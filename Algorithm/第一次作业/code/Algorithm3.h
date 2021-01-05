#pragma once
#include "ustime.h"
class Algorithm3
{
public:
	Algorithm3(int, int*);
	~Algorithm3();
	long long Run();
	long long Work(int, int);
private:
	int N;
	int* Data;
	long long RunTime;
};