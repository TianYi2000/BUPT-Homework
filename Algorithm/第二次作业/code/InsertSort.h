#pragma once
#include "Data.h"
#include "Branch.h"
#include "ustime.h"
#define LL long long
class InsertSort
{
public:
	InsertSort(int, Data*);
	~InsertSort();
	void Run();
	LL CountMove();
	LL CountCompare();
	void showBranchTest();
	double RunTime;
private:
	int N;
	Data* A;
	Branch branch;
	int b1, b2, b3;	//(j >= 0) && (A[j] > key)
};

