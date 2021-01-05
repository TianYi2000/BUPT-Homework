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
	srand((int)time(0));  // �����������
	for (int i = 0; i < N; i++)
	{
		data[i] = (rand() % 200) - 100; //���� -100~99 �������
	}
	return data;
}

DataCreate::~DataCreate()
{
	delete[] data;
}
