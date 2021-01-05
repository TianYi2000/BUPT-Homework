#include "Data.h"
Data::Data()
{
	MoveTimes = 0;
	CompareTimes = 0;
	Value = 0;
}

void Data::Init(const int _Value)
{
	Value = _Value;
	MoveTimes = 0;
	CompareTimes = 0;
}

void Data::operator=(const int _Value)
{
	Value = _Value;
	MoveTimes++;
}

void Data::operator=(const Data& rhs)
{
	Value = rhs.Value;
	MoveTimes++;
}

bool Data::operator<(const int _Value)
{
	CompareTimes++;
	return Value < _Value;
}

bool Data::operator<(const Data& rhs)
{
	CompareTimes++;
	return Value < rhs.Value;

}
bool Data::operator>(const int _Value)
{
	CompareTimes++;
	return Value > _Value;
}

bool Data::operator>(const Data& rhs)
{
	CompareTimes++;
	return Value > rhs.Value;
}

bool Data::operator<=(const int _Value)
{
	CompareTimes++;
	return Value <= _Value;
}

bool Data::operator<=(const Data& rhs)
{
	CompareTimes++;
	return Value <= rhs.Value;

}
bool Data::operator>=(const int _Value)
{
	CompareTimes++;
	return Value >= _Value;
}

bool Data::operator>=(const Data& rhs)
{
	CompareTimes++;
	return Value >= rhs.Value;
}

Data::~Data()
{

}