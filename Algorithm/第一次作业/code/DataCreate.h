#pragma once
class DataCreate
{
public:
	DataCreate(int);
	~DataCreate();
	int* GetData();
private:
	int N;	//需要随机生成的数据量
	int* data;	//随机生成的数据数组
};

