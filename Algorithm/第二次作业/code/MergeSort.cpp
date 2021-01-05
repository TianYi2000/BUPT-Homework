#include "MergeSort.h"
#include <iostream>
#define Reg register
#define gmax(_a, _b) ((_a) > (_b) ? (_a) : (_b))
#define gmin(_a, _b) ((_a) < (_b) ? (_a) : (_b))
#define cmax(_a, _b) (_a < (_b) ? _a = (_b) : 0)
#define cmin(_a, _b) (_a > (_b) ? _a = (_b) : 0)
#define FOR(_i,_a,_b) for (Reg int _i = (_a) ; _i <= (_b) ; _i++)
#define REP(_i,_a,_b) for (Reg int _i = (_a) ; _i >= (_b) ; _i--)
using namespace std;

MergeSort::MergeSort(int n, Data* _A)
{
	N = n;
	A = _A;

	b1 = branch.AddBranch("(i <= m) = True , (j <= r) = True");
	b2 = branch.AddBranch("(i <= m) = True , (j <= r) = Flase");
	b3 = branch.AddBranch("(i <= m) = False , (j <= r) = True");

	b5 = branch.AddBranch("(c[i] <= c[j]) = True");
	b6 = branch.AddBranch("(c[i] <= c[j]) = Flase");

	b7 = branch.AddBranch("(i > m) = True");
	b8 = branch.AddBranch("(i > m) = Flase");

	b9 = branch.AddBranch("(i + 2 * s - 1 < n) = True");
	b10 = branch.AddBranch("(i + 2 * s - 1 < n) = Flase");

	b11 = branch.AddBranch("(i + s - 1 < n - 1) = True");
	b12 = branch.AddBranch("(i + s - 1 < n - 1) = Flase");

	b13 = branch.AddBranch("(s < N) = True");
	b14 = branch.AddBranch("(s < N) = Flase");
}

void MergeSort::Merge(Data* c, Data* d, int l, int m, int r)
{
	int i = l, j = m + 1, k = l;
//	FOR(i, l, m) cout << c[i].Value << " "; cout << "|";
//	FOR(i, m + 1, r) cout << c[i].Value << " ";
//	cout << "C=" << CountCompare();
//	cout << endl;
	while ((i <= m) && (j <= r))
	{
		branch.Pass(b1);
		if (c[i] <= c[j])
		{
			branch.Pass(b5);
			d[k++] = c[i++];
		}			
		else
		{
			branch.Pass(b6);
			d[k++] = c[j++];
		}
	}
	if ((i <= m) && !(j <= r)) branch.Pass(b2);
	if (!(i <= m) && (j <= r)) branch.Pass(b3);

	if (i > m)
	{
		branch.Pass(b7);
		FOR(q, j, r)
			d[k++] = c[q];
	}
	else
	{
		branch.Pass(b8);
		FOR(q, i, m)
			d[k++] = c[q];
	}
}

void MergeSort::MergePass(Data* x, Data* y, int s, int n)
{
	int i = 0;
	while (i + 2 * s - 1 < n)
	{
		branch.Pass(b9);
		Merge(x, y, i, i + s - 1, i + 2 * s - 1);
		i = i + 2 * s;
	}
	branch.Pass(b10);
	if (i + s - 1 < n - 1 )
	{
		branch.Pass(b11);
		Merge(x, y, i, i + s - 1, n - 1);
	}
	else
	{
		branch.Pass(b12);
		FOR(j, i, n - 1)
			y[j] = x[j];
	}
}

void MergeSort::Run()
{
	B = new Data[N];
	double BeginTime = GetSysTimeMicros();
	int s = 1;
	while(s < N)
	{
		branch.Pass(b13);
		MergePass(A, B, s, N);
		s += s;
		if (s >= N)
		{
			for (int i = 0; i < N; ++i)
				A[i].Value = B[i].Value;
		}
		else
		{
			MergePass(B, A, s, N);
		}
		s += s;
	}
	branch.Pass(b14);
	RunTime = (GetSysTimeMicros() - BeginTime) / 1000.0;
}

LL MergeSort::CountMove()
{
	LL Sum = 0;
	FOR(i, 0, N - 1)
		Sum += A[i].MoveTimes + B[i].MoveTimes;
	return Sum;
}

LL MergeSort::CountCompare()
{
	LL Sum = 0;
	FOR(i, 0, N - 1)
		Sum += A[i].CompareTimes + B[i].CompareTimes;
	return Sum;
}

void MergeSort::showBranchTest()
{
	branch.showResult();
}

MergeSort::~MergeSort()
{
	delete[] B;
}