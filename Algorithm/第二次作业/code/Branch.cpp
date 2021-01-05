#include "Branch.h"
#include <iostream>
Branch::Branch()
{
	tot_branch = 0;
}

Branch::~Branch()
{

}

int Branch::AddBranch(string condition)
{
	branch[++tot_branch] = BranchStruct(condition);
	return tot_branch;
}

void Branch::Pass(int id)
{
	branch[id].passed = 1;
}

void Branch::showResult()
{
	int allPassed = 1;
	cout << "分支遍历结果如下:" << endl;
	for (int i = 1; i <= tot_branch; ++i)
	{
		if (branch[i].passed == 0)
		{
			allPassed = 0;
			cout << branch[i].branch_name << " : " << "×" << endl;
		}
		else
			cout << branch[i].branch_name << " : " << "√" << endl;
	}
	if (allPassed == 1)
		cout << "Congratulations! 通过了所有分支:)" << endl;
	else
	{
		cout << "没有所有分支:(  未通过的分支如下:" << endl;
		for (int i = 1; i <= tot_branch; ++i)
			if (branch[i].passed == 0)
				cout << branch[i].branch_name << endl;
	}
	return;
}