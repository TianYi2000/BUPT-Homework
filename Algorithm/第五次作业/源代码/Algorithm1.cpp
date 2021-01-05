#include "Algorithm1.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#define DEBUG

#define gmax(_a, _b) ((_a) > (_b) ? (_a) : (_b))
Algorithm1::Algorithm1(int f, int v, int** n)
{
	F = f;
	V = v;
	N = n;
	M = new int* [F + 1];
	
	for (int i = 0; i <= F; ++i)
		M[i] = new int[V + 1];
	for (int i = 0; i <= F; ++i)
		for (int j = 0; j <= V; ++j)
			M[i][j] = 0;

	Result = new int [F + 1];
	for (int i = 1; i <= F; ++i)
		Result[i] = 0;
}

int Algorithm1::Run()
{
	long long StartTime, EndTime;
	StartTime = GetSysTimeMicros();

	for (int i = 1; i <= F; ++i)
		for (int j = i; j <= V; ++j)
			M[i][j] = gmax(M[i][j - 1], M[i - 1][j - 1] + N[i][j]);

	for (int i = F, j = V; i >= 1; j--)
	{
		if (M[i][j] > M[i][j - 1])
		{
			Result[i] = j;
			i --;
		}
	}

	EndTime = GetSysTimeMicros();
	RunTime = EndTime - StartTime;
	return M[F][V];
}

void Algorithm1::Print()
{
#ifdef DEBUG
	cout << "DP表：" << endl;
	cout << setw(8) << "花束\\花瓶";
	for (int i = 1; i <= V; ++i)
		cout << setw(5) << i;
	cout << endl;
	for (int i = 1; i <= F; ++i)
	{
		cout << setw(9) << i;
		for (int j = 1; j <= V; ++j)
			cout << setw(5) << M[i][j];
		cout << endl;
	}
#endif // DEBUG

	
	cout << "最大美学值为: " << M[F][V] << endl;
	cout << "摆花方案为: " << endl;
	for (int i = 1; i <= F; ++i)
		cout << "第 " << i << " 束花被摆放到第 " << Result[i] << " 个花瓶" << endl;
	cout << "运行时间: " << RunTime / 1000000.0 << " s" << endl;
}

Algorithm1::~Algorithm1()
{
	for (int i = 0; i <= F; ++i)
		delete[] M[i];
	delete[] M;

	delete[] Result;
}

