#pragma once
#include "ustime.h"
class Algorithm4
{
public:
	Algorithm4(int, int*);
	~Algorithm4();
	long long Run();
private:
	int N;
	int* Data;
	long long RunTime;
};

