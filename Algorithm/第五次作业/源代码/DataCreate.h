#pragma once
class DataCreate
{
public:
	DataCreate(int, int);
	~DataCreate();
	int** GetData();
	void Print();
private:
	int F, V;	//��Ҫ������ɵ�������
	int** data;	//������ɵ���������
};

