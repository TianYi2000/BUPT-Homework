#pragma once
class DataCreate
{
public:
	DataCreate(int);
	~DataCreate();
	int* GetData();
private:
	int N;	//��Ҫ������ɵ�������
	int* data;	//������ɵ���������
};

