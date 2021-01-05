#include "InsertSort.h"
#define Reg register
#define gmax(_a, _b) ((_a) > (_b) ? (_a) : (_b))
#define gmin(_a, _b) ((_a) < (_b) ? (_a) : (_b))
#define cmax(_a, _b) (_a < (_b) ? _a = (_b) : 0)
#define cmin(_a, _b) (_a > (_b) ? _a = (_b) : 0)
#define FOR(_i,_a,_b) for (Reg int _i = (_a) ; _i <= (_b) ; _i++)
#define REP(_i,_a,_b) for (Reg int _i = (_a) ; _i >= (_b) ; _i--)


InsertSort::InsertSort(int n, Data* _A)
{
	N = n;
	A = _A;

	b1 = branch.AddBranch("(j >= 0) = True , (A[j] > key) = True");
	b2 = branch.AddBranch("(j >= 0) = True , (A[j] > key) = Flase");
	b3 = branch.AddBranch("(j >= 0) = False");	//因j < 0时 A[j]无意义，此时无需考虑A[j]的情况
}

void InsertSort::Run()
{
	long long BeginTime = GetSysTimeMicros();
	FOR (i, 1, N - 1) {
		int key = A[i].Value;
		int j = i - 1;
		while ((j >= 0) && (A[j] > key)) {
			A[j + 1] = A[j];
			j--;
		}
		A[j + 1] = key;
	}
	RunTime = (GetSysTimeMicros() - BeginTime) / 1000.0;
}

LL InsertSort::CountMove()
{
	LL Sum = 0;
	FOR(i, 0, N - 1)
		Sum += A[i].MoveTimes;
	return Sum;
}

LL InsertSort::CountCompare()
{
	LL Sum = 0;
	FOR(i, 0, N - 1)
		Sum += A[i].CompareTimes;
	return Sum;
}

void InsertSort::showBranchTest()
{
	branch.showResult();
}

InsertSort::~InsertSort()
{

}