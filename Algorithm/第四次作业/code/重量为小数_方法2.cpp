#include <iostream>
using namespace std;
const int maxn = 1000, maxm = 10000;
const double INF = 1e7;
const double eps = 1e-6;
double W[maxn + 10], F[maxm];
int V[maxn + 10];
int main()
{
	int N;
	double M;
	int maxv = 0;
	cin >> N >> M;
	for (int i = 1; i <= N; i++)
	{
		cin >> W[i] >> V[i];
		maxv += V[i];
	}
	for (int i = 1;  i <= maxv; ++i) F[i] = INF;
	F[0] = 0;
	for (int i = 1; i <= N; i++)
		for (int j = maxv; j >= V[i]; j--)
		if (F[j - V[i]] + W[i] - eps < F[j]) F[j] = F[j - V[i]] + W[i];
	for (int i = maxv; i >= 0; i--)
		if (F[i] - eps <= M)
		{
			cout << i << endl;
			break;
		}
	return 0;
}