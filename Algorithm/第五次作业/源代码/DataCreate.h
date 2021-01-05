#pragma once
class DataCreate
{
public:
	DataCreate(int, int);
	~DataCreate();
	int** GetData();
	void Print();
private:
	int F, V;	//需要随机生成的数据量
	int** data;	//随机生成的数据数组
};

