#include "Table.h"
#include <iostream>
#define Reg register
#define FOR(_i,_a,_b) for (Reg int _i = (_a) ; _i <= (_b) ; _i++)
Table::Table(int _N)
{
	N = _N;
	n_is_even = (N & 1) ? 0 : 1;
	date = new int* [N + 2];
	FOR(i, 0, N + 1)
		date[i] = new int[N + 2];
	has_fought = new int* [N + 2];
	FOR(i, 0, N + 1)
		has_fought[i] = new int[N + 2];
	FOR (i, 1, N + 1)
		FOR(j, 1, N + 1) {
		date[i][j] = has_fought[i][j] = 0;
	}
}

Table::~Table()
{
	FOR(i, 0, N + 1) {
		delete[] date[i];
		delete[] has_fought[i];
	}
	delete[] date;
	delete[] has_fought;
}

void Table::PrintTable()
{
	FOR(day, 1, N - n_is_even)
	{
		for (auto i = record[day].begin(); i != record[day].end(); i++)
		{
			int x = (*i).x;
			int y = (*i).y;
			cout << "(" << x << " , " << y << ") ";
		}
		cout << endl;
	}
	FOR(i, 1, N - n_is_even) {
		FOR(j, 1, N)
			cout << date[i][j] << " ";
		cout << endl;
	}
}

bool Table::CheckRight()
{
	FOR(i, 1, N)
		FOR(j, 1, N)
		{
		has_fought[i][j] =  (i == j);
		}
			
	FOR(day, 1, N - n_is_even)
		FOR(x, 1, N)
		if (date[day][x] != 0)		//Ã»ÓÐ±»ÂÖ¿Õ
		{
		int y = date[day][x];
		if (has_fought[x][y] || date[day][y] != x)
			return false;
		has_fought[x][y] = 1;
		}
	FOR(i, 1, N)
		FOR(j, 1, N)
		if (has_fought[i][j] == 0)
			return false;
	return true;
}