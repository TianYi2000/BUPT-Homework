#include "Rotate.h"
#include <iostream>
#define Reg register
#define FOR(_i,_a,_b) for (Reg int _i = (_a) ; _i <= (_b) ; _i++)
using namespace std;
Rotate::Rotate(int _N) : table(_N), N(_N) {
	n_is_even = (N & 1) ? 0 : 1;
	FOR(i, 1, N - n_is_even)
		vertex.push_back(i);
};

void Rotate::Run()
{
	long long BeginTime = GetSysTimeMicros();
	FOR(day, 1, N - n_is_even)
	{
		FOR(i, 0, (N - 1) / 2 - 1)
		{
			int x = vertex[i];
			int y = vertex[N - n_is_even - 1 - i];
			table.record[day].push_back(Fight(x, y));
			table.date[day][x] = y;
			table.date[day][y] = x;
		}
		int mid = vertex[(N - 1) / 2];
		if (n_is_even)
		{
			table.record[day].push_back(Fight(mid, N));
			table.date[day][mid] = N;
			table.date[day][N] = mid;
		}
		else
		{
			table.record[day].push_back(Fight(mid, 0));
			table.date[day][mid] = 0;
		}
		int tail = vertex[N - n_is_even - 1];
		vertex.pop_back();
		vertex.insert(vertex.begin(), tail);
	}
	RunTime = (GetSysTimeMicros() - BeginTime) / 1000.0;
}
