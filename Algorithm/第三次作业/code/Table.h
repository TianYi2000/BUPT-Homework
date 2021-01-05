#pragma once
#include <vector>
using namespace std;
const int MAXN = 10000;
struct Fight
{
	int x, y;
	Fight() : x(0), y(0) {};
	Fight(int _x, int _y) : x(_x), y(_y) {};
};
class Table
{
public:
	Table(int);
	~Table();
	void PrintTable();
	bool CheckRight();
	int N, n_is_even;
	int** date;
	int** has_fought;
	vector <Fight> record[MAXN];
};

