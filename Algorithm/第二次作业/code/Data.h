#pragma once
class Data
{
public:
	Data();
	~Data();
	int Value, MoveTimes, CompareTimes;
	void Init(const int);
	void operator=(const int);
	void operator=(const Data&);
	bool operator<(const int);
	bool operator<(const Data&);
	bool operator>(const int);
	bool operator>(const Data&);
	bool operator<=(const int);
	bool operator<=(const Data&);
	bool operator>=(const int);
	bool operator>=(const Data&);
};

