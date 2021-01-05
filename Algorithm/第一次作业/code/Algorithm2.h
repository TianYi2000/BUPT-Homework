#pragma once
#include "ustime.h"
class Algorithm2
{
public:
	Algorithm2(int, int*);
	~Algorithm2();
	long long Run();
private:
	int N;
	int* Data;
	long long RunTime;
};

