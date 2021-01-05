#pragma once
#include "Data.h"
class DataCreate
{
public:
	enum SequenceType{
		SmallToLarge = 0,	//��С�������������
		Random,				//�������
		LargeToSmall,		//�Ӵ�С����������
		MiddleInFirst,		//���м�������ڵ�һ��������
		Sepa				//��������
	};
	DataCreate(int, SequenceType _type = Random);
	~DataCreate();
	Data* GetData();
	static int Time;
private:
	void Adjust(int, int, int);
	void Swap(Data&, Data&);
	void Separate();
	void anti_MergePass(int );
	void anti_Merge(int, int, int);
	int N;	//��Ҫ������ɵ�������
	SequenceType type; //��Ҫ������ɵ���������
	Data* data;	//������ɵ���������
	int* A;//��������
};

