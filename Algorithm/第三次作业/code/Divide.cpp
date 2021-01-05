#include "Divide.h"
#define Reg register
#define FOR(_i,_a,_b) for (Reg int _i = (_a) ; _i <= (_b) ; _i++)
void Divide::Run()
{
	long long BeginTime = GetSysTimeMicros();
	work(N);
	RunTime = (GetSysTimeMicros() - BeginTime) / 1000.0;
}

void Divide::work(int n)
{	
	if (n == 2)
	{
		table.date[1][1] = 2;
		table.date[1][2] = 1;
		return;
	}
	int n_is_odd = n & 1;
	int n_is_even = 1 - n_is_odd;

	int sub_n = (n + n_is_odd) / 2;
	int sub_n_is_odd = sub_n & 1;
	int sub_n_is_even = 1 - sub_n_is_odd;

	work(sub_n);
	FOR(i, 1, sub_n - sub_n_is_even)
		FOR(j, sub_n + 1, n)
		if (table.date[i][j - sub_n] == 0)
		{
			table.date[i][j - sub_n] = j;
			table.date[i][j] = j - sub_n;
		}
		else
			table.date[i][j] = table.date[i][j - sub_n] + sub_n;		
	
	FOR(j, 1, sub_n)
	{
		int fight = j + sub_n;
		if (sub_n_is_odd)
		{
			fight++;
			if (fight > n + n_is_odd)
				fight = sub_n + 1;
		}

		FOR(i, sub_n + sub_n_is_odd, n - n_is_even)
		{

			table.date[i][j] = fight;
			if (fight <= n)	table.date[i][fight] = j;
			
			fight++;
			if (fight > n + n_is_odd)
				fight = sub_n + 1;
		}
	}

	if (n_is_odd)
	{
		FOR(i, 1, n - n_is_even)
			FOR(j, 1, n)
			if (table.date[i][j] == n + n_is_odd)
				table.date[i][j] = 0;
	}
		
			
}