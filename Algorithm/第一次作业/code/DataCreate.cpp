#include "DataCreate.h"
#include <cstdlib>
#include <ctime>

DataCreate::DataCreate(int n)
{
	N = n;
	data = new int[N];
}

int* DataCreate::GetData()
{
	srand((int)time(0));  // 产生随机种子
	for (int i = 0; i < N; i++)
	{
		data[i] = (rand() % 200) - 100; //产生 -100~99 的随机数
	}
	return data;
}

DataCreate::~DataCreate()
{
	delete[] data;
}
