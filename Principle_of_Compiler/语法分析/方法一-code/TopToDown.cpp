#include <iostream>
#include <string>
using namespace std;
string S;
int point = 0;
void procE();
void procF();
void procT();
void end(int result)
{
	if (result == 1)
		cout << "分析完成，无错误";
	else
	{
		cout << "发生错误!" << endl;
		cout << "错误出现在字符串'" << S.substr(0, point) << "'末尾处" << endl;
	}
	exit(0);
}
void procE()
{
	procT();
	if (S[point] ==  '+' || S[point] == '-') {
		point++;
		procE();
	}
}
void procT() 
{
	procF();
	if (S[point] == '*' || S[point] == '/') {
		point++;
		procT();
	}
}
void procF() {
	if (S[point] == '(')
	{
		point++;
		procE();
		if (S[point] == ')')
			point++;
		else
			end(0);
	}
	else if (S[point] >= '0' && S[point] <= '9')
	{
		while (S[point] >= '0' && S[point] <= '9')
			point++;
	}
	else
		end(0);
}
int main()
{
	cin >> S;
	S += '$';
	procE();
	if (S[point] == '$')
		end(1);
	else
		end(0);
}