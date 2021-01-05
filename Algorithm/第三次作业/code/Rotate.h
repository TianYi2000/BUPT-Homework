#pragma once
#include "Table.h"
#include "ustime.h"
#include <vector>
using namespace std;
class Rotate
{
public:
	Table table;
	Rotate(int _N);
	void Run();
	void PrintResult() { table.PrintTable(); };
	bool CheckRight() { return table.CheckRight(); };
	double RunTime;
private:
	int N, n_is_even;
	vector<int> vertex;
};