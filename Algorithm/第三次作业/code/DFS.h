#pragma once
#include "Table.h"
#include "ustime.h"
class DFS
{
public:
	Table table;
	DFS(int _N) : table(_N), N(_N) {};
	void Run();

	void PrintResult() { table.PrintTable(); };
	bool CheckRight() { return table.CheckRight(); };
	double RunTime;
private:
	int N;
	inline void fight(int day, int x, int y);
	inline void cancel_fight(int day, int x, int y);
	bool dfs_day(int day, int rest);
	bool dfs_arrange(int day, int player, int rest);
};