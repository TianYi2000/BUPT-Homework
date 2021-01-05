#pragma once
#include "ustime.h"
using namespace std;
class Algorithm1
{
public:
	Algorithm1(int, int, int**);
	~Algorithm1();
	int Run();
	void Print();
	long long RunTime;
private:
	int F, V;
	int** N;
	int** M;
	int* Result;
};

