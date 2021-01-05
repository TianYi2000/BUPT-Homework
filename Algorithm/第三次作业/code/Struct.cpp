#include "Struct.h"
#define Reg register
#define FOR(_i,_a,_b) for (Reg int _i = (_a) ; _i <= (_b) ; _i++)

Struct::Struct(int _N) : table(_N), N(_N) {
	temp = new int[N + 2];
	FOR(i, 1, N + 1) temp[i] = 0;
	n_is_even = (N & 1) ? 0 : 1;
	int x = 1, y = N, i = 0;
	while (x <= y)
	{
		if (x == y)
		{
			temp[++i] = x;
			temp[++i] = 0;
			table.date[1][x] = 0;
		}		
		else
		{
			temp[++i] = x;
			temp[++i] = y;
			table.date[1][x] = y;
			table.date[1][y] = x;
		}		
		x++ ;
		y-- ; 
	}
};

void Struct::Run()
{
	long long BeginTime = GetSysTimeMicros();
	FOR(j, 2, N - n_is_even)
	{
		for (int i = 1; i <= N; i+= 2)
		{
			int x = temp[i];
			int y = temp[i + 1];
			x = (x == N - n_is_even) ? 1 : (x + 1);
			if (y != 0 && !(n_is_even && y == N))
				y = (y == N - n_is_even) ? 1 : (y + 1);
			temp[i] = x;
			temp[i + 1] = y;
			table.date[j][x] = y;
			table.date[j][y] = x;
		}
	}
	RunTime = (GetSysTimeMicros() - BeginTime ) / 1000.0;;
}

Struct::~Struct()
{
	delete[] temp;
}