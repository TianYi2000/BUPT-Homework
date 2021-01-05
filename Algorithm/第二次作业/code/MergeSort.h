#pragma once
#include "Data.h"
#include "Branch.h"
#include "ustime.h"
#define LL long long
class MergeSort
{
public:
	MergeSort(int, Data*);
	~MergeSort();
	void Run();
	void MergePass(Data*, Data*, int s, int n);
	void Merge(Data*, Data*, int, int, int);
	LL CountMove();
	LL CountCompare();
	void showBranchTest();
	double RunTime;
private:
	int N;
	Data *A, *B;
	Branch branch;
	int b1, b2, b3,				//i <= m && j <= r
		b5, b6,						//c[i] <= c[j]
		b7, b8,						//i > m
		b9, b10,					//i + 2 * s - 1 < n
		b11, b12,					//i + s - 1 < n - 1
		b13, b14;					//s < N
};

