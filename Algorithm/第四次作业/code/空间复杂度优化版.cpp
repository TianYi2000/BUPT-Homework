#include <iostream>
using namespace std;
const int maxn = 1000, maxm = 10000;
int W[maxn + 10], V[maxn + 10], F[maxm];
int main()
{
	int N, M;
	cin >> N >> M;
	for (int i = 1; i <= N; i++) cin >> W[i] >> V[i];
	for (int i = 1; i <= N; i++)
		for (int j = M; j >= W[i]; j--)
		if (F[j - W[i]] + V[i] > F[j]) F[j] = F[j - W[i]] + V[i];
	cout << F[M];
	return 0;
}