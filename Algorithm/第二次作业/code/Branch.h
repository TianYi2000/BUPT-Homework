#pragma once
#include <string>
#include <vector>
using namespace std;
class Branch
{
public:
	struct BranchStruct
	{
		string branch_name;
		int passed;
		BranchStruct() : branch_name(""), passed(0){};
		BranchStruct(string _name) : 
			branch_name(_name), passed(0){};
	};
	Branch();
	~Branch();
	int AddBranch(string);
	void Pass(int);
	void showResult();

	int tot_branch;

	BranchStruct branch[100];

};

