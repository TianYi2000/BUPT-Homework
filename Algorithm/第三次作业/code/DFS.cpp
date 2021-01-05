#include "DFS.h"
#include <iostream>
//#define DEBUG
#define Reg register
#define FOR(_i,_a,_b) for (Reg int _i = (_a) ; _i <= (_b) ; _i++)
using namespace std;
void DFS::Run()
{
	long long BegieTime = GetSysTimeMicros();
	dfs_day(1, (N & 1) ? N : 0);
	
	RunTime = (GetSysTimeMicros() - BegieTime) / 1000.0;
}

bool DFS::dfs_day(int day, int rest)
{
	if (day > N - 1 + (N & 1))
		return true;
	return dfs_arrange(day, 1, rest);
}

inline void DFS::fight(int day, int x, int y)
{
	table.date[day][x] = y;
	table.date[day][y] = x;
	table.has_fought[x][y] = 1;
	table.has_fought[y][x] = 1;
}

inline void DFS::cancel_fight(int day, int x, int y)
{
	table.date[day][x] = 0;
	table.date[day][y] = 0;
	table.has_fought[x][y] = 0;
	table.has_fought[y][x] = 0;
}

bool DFS::dfs_arrange(int day, int player, int rest)
{
	if (player > N) {
		return dfs_day(day + 1, rest - 1);
	}
	if (player == rest || table.date[day][player])	//如果当前player在第day天被轮空，或在之前已经被安排好比赛，则直接考虑下一人
		return dfs_arrange(day, player + 1, rest);
	FOR(i, player + 1, N)
		if (!table.date[day][i] && player != rest && !table.has_fought[player][i])
		{
			fight(day, player, i);			//安排player与i比赛
			if (dfs_arrange(day, player + 1, rest))		//如果在当前的安排下深搜结果为TRUE，则不必继续深搜
				return true;										//直接返回可行结果
			cancel_fight(day, player, i);	//撤销player与i的比赛记录
		}
	return false;	//当前运动员遍历完其他所有人都没有找到合法对手，说明当前安排不可行
}
