#pragma once
#include "Data.h"
#include "Branch.h"
#include "ustime.h"
#define LL long long
class QuickSort
{
public:
	QuickSort(int, Data*);
	~QuickSort();
	void Swap(Data&, Data&);
	void Run();
	void Sort(int, int);
	LL CountMove();
	LL CountCompare();
	void showBranchTest();
	double RunTime;
private:
	int N;
	Data* A;
	Branch branch;
	int b1, b2,			//l >= r
		b3, b4;			//A[j] <= x
};

