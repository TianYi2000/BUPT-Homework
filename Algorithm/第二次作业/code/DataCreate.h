#pragma once
#include "Data.h"
class DataCreate
{
public:
	enum SequenceType{
		SmallToLarge = 0,	//从小到大的有序数列
		Random,				//随机数列
		LargeToSmall,		//从大到小的有序数列
		MiddleInFirst,		//最中间的数字在第一个的序列
		Sepa				//均分两边
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
	int N;	//需要随机生成的数据量
	SequenceType type; //需要随机生成的序列类型
	Data* data;	//随机生成的数据数组
	int* A;//辅助数组
};

