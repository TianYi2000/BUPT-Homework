#include "QuickSort.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#define Reg register
#define gmax(_a, _b) ((_a) > (_b) ? (_a) : (_b))
#define gmin(_a, _b) ((_a) < (_b) ? (_a) : (_b))
#define cmax(_a, _b) (_a < (_b) ? _a = (_b) : 0)
#define cmin(_a, _b) (_a > (_b) ? _a = (_b) : 0)
#define FOR(_i,_a,_b) for (Reg int _i = (_a) ; _i <= (_b) ; _i++)
#define REP(_i,_a,_b) for (Reg int _i = (_a) ; _i >= (_b) ; _i--)


QuickSort::QuickSort(int n, Data* _A)
{
	N = n;
	A = _A;

	b1 = branch.AddBranch("(l >= r) = True");
	b2 = branch.AddBranch("(l >= r) = Flase");

	b3 = branch.AddBranch("(A[j] <= x) = True");
	b4 = branch.AddBranch("(A[j] <= x) = Flase");
}

void QuickSort::Run()
{
	double BeginTime = GetSysTimeMicros();
	srand((int)time(0));
	Sort(0, N - 1);
	RunTime = (GetSysTimeMicros() - BeginTime) / 1000.0;
}

void QuickSort::Swap(Data& x, Data& y)
{
	int temp = x.Value;
	x = y;
	y = temp;
}

void QuickSort::Sort(int l, int r)
{
	
	if (l >= r)
	{
		A[0] > -1;
		branch.Pass(b1);
		return;
	}
//	for (int i = l; i <= r; ++i) cout << A[i].Value << " ";
//	cout << endl;
	branch.Pass(b2);
	int i, x, base = l;

	/*¹Ø±ÕÓÅ»¯*/
	base = l + rand() % (r - l + 1);
	if (l != base) Swap(A[l], A[base]);

	x = A[l].Value;
	i = l;
	FOR(j, l + 1, r)
	{
		
		if (A[j] <= x)
		{
			branch.Pass(b3);
			i++;
			if (i != j) Swap(A[i], A[j]);
		}
		else
		{
			branch.Pass(b4);
		}
	}
		
	Swap(A[l], A[i]);
//	std::cout << l << ", " << i << ", " << r << std::endl;
	Sort(l, i - 1);
	Sort(i + 1, r);
}

LL QuickSort::CountMove()
{
	LL Sum = 0;
	FOR(i, 0, N - 1)
		Sum += A[i].MoveTimes;
	return Sum;
}

LL QuickSort::CountCompare()
{
	LL Sum = 0;
	FOR(i, 0, N - 1)
		Sum += A[i].CompareTimes;
	return Sum;
}

void QuickSort::showBranchTest()
{
	branch.showResult();
}

QuickSort::~QuickSort()
{

}