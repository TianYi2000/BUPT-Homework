#include "DataCreate.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

DataCreate::DataCreate(int f, int v)
{
	F = f;
	V = v;
	data = new int* [F + 1];
	for (int i = 0; i <= F; ++i)
		data[i] = new int[V + 1];
}

int** DataCreate::GetData()
{
	srand(time(NULL));
	for (int i = 1; i <= F; i++)
		for (int j = 1; j <= V; j++)
		{
			data[i][j] = rand() % 100;
		}
	return data;
}

void DataCreate::Print()
{
	cout << "美学值表：" << endl;
	cout << setw(8) << "花束\\花瓶";
	for (int i = 1; i <= V; ++i)
		cout << setw(5) << i;
	cout << endl;
	for (int i = 1; i <= F; ++i)
	{
		cout << setw(9) << i;
		for (int j = 1; j <= V; ++j)
			cout << setw(5) << data[i][j];
		cout << endl;
	}
}

DataCreate::~DataCreate()
{
	for (int i = 0; i <= F; ++i)
		delete[] data[i];
	delete[] data;
}
