#pragma once
#include "ustime.h"
class Algorithm1
{
public:
	Algorithm1(int, int*);
	~Algorithm1();
	long long Run();
private:
	int N;
	int* Data;
	long long RunTime;
};

