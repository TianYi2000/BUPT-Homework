#pragma once
#include "Table.h"
#include "ustime.h"
class Divide
{
public:
	Table table;
	Divide(int _N) : table(_N), N(_N) {};
	void Run();
	void PrintResult() { table.PrintTable(); };
	bool CheckRight() { return table.CheckRight(); };
	double RunTime;

private:
	int N;
	void work(int);
};

