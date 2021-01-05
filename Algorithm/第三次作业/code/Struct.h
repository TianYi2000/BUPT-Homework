#pragma once
#include "Table.h"
#include "ustime.h"
class Struct
{
public:
	Table table;
	Struct(int _N);
	~Struct();
	void Run();
	void PrintResult() { table.PrintTable(); };
	bool CheckRight() { return table.CheckRight(); };
	double RunTime;
private:
	int N, n_is_even;
	int* temp;
};

