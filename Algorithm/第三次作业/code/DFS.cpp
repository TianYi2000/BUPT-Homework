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
	if (player == rest || table.date[day][player])	//�����ǰplayer�ڵ�day�챻�ֿգ�����֮ǰ�Ѿ������źñ�������ֱ�ӿ�����һ��
		return dfs_arrange(day, player + 1, rest);
	FOR(i, player + 1, N)
		if (!table.date[day][i] && player != rest && !table.has_fought[player][i])
		{
			fight(day, player, i);			//����player��i����
			if (dfs_arrange(day, player + 1, rest))		//����ڵ�ǰ�İ��������ѽ��ΪTRUE���򲻱ؼ�������
				return true;										//ֱ�ӷ��ؿ��н��
			cancel_fight(day, player, i);	//����player��i�ı�����¼
		}
	return false;	//��ǰ�˶�Ա���������������˶�û���ҵ��Ϸ����֣�˵����ǰ���Ų�����
}
