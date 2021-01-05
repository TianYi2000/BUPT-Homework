#include "DataCreate.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;
DataCreate::DataCreate(int n, SequenceType _type)
{
	N = n;
	data = new Data[N];
	type = _type;
}

Data* DataCreate::GetData()
{
	Time++;
	srand((int)(time(0)+ Time));  // 产生随机种子
	switch (type)
	{
	case DataCreate::SmallToLarge:
		for (int i = 0; i < N; i++)
			data[i] = i + 1;
		break;
	case DataCreate::Random:
		for (int i = 0; i < N; i++)
		{
			data[i].Init(rand() % 100000000);
		}
		break;
	case DataCreate::LargeToSmall:
		for (int i = 0; i < N; i++)
			data[i] = N - i;
		break;
	case DataCreate::MiddleInFirst:
		for (int i = 0; i < N; i++)
			data[i] = i + 1;
		Adjust(0, N - 1, 1);
		for (int i = 0; i < N; i++)
		{
			data[i].CompareTimes = 0;
			data[i].MoveTimes = 0;
		}
		break;
	case DataCreate::Sepa:
		for (int i = 0; i < N; i++)
			data[i] = i + 1;
		Separate();
		for (int i = 0; i < N; i++)
		{
			data[i].CompareTimes = 0;
			data[i].MoveTimes = 0;
		}
		break;

	default:
		break;
	}
	return data;
}

void DataCreate::Swap(Data& x, Data& y)
{
	int temp = x.Value;
	x = y;
	y = temp;
}

void DataCreate::Adjust(int l, int r, int belong)
{
	if (l >= r)
		return;
	int base = (l + r) / 2;
	if (belong == 0)
	{
		Swap(data[r], data[base]);
		if (l < r)
			sort(data + l, data + r - 1);
		Adjust(l, base - 1, 0);
		Adjust(base, r - 1, 1);
	}		
	else
	{
		Swap(data[l], data[base]);
		if (l < r)
			sort(data + l + 1, data + r);
		Adjust(l + 1, base, 0);
		Adjust(base + 1, r, 1);
	}	
}

void DataCreate::anti_Merge(int l, int m, int r)
{
	int i = m, j = r, k = r;
	while ((i >= l) && (j >= m + 1))
	{
		A[i--] = data[k--].Value;
		A[j--] = data[k--].Value;
	}

	if (i < l)
	{
		for (int q = j; q >= m + 1; q--)
			A[q] = data[k--].Value;
	}
	else
	{
		for (int q = i; q >= l; q--)
			A[q] = data[k--].Value;
	}
	for (int i = l; i <= r; ++i)
		data[i] = A[i];
}

void DataCreate::anti_MergePass(int s)
{
	int i = 0;
	while (i + 2 * s - 1 < N)
	{
		anti_Merge(i, i + s - 1, i + 2 * s - 1);
		i = i + 2 * s;
	}
	if (i + s - 1 < N - 1)
	{
		anti_Merge(i, i + s - 1, N - 1);
	}

}



void DataCreate::Separate()
{
	A = new int[N];
	int maxS = 1;
	while (maxS < N) maxS *= 2;
	maxS /= 2;
	int s = maxS;
	while (s >= 1)
	{
		anti_MergePass(s);
		s /= 2;
	}
	delete[] A;
}

DataCreate::~DataCreate()
{
	delete[] data;
}
